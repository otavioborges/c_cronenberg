#include <stdlib.h>
#include <time.h>
#include "CronenbergController.h"

// Packet Types
#include "PacketPayload.h"
#include "PacketType.h"
#include "DataPacket.h"
#include "AckNack.h"
#include "PingPong.h"
#include "RequestID.h"
#include "ResponseID.h"

using namespace std;
using namespace cronenberg;

typedef vector<pair<CronenbergPacket *, uint32_t>>::iterator outputIt;
typedef vector<CronenbergPacket *>::iterator inputIt;
typedef pair<CronenbergPacket *, uint32_t> outPair;
typedef set<NodeInfo *>::iterator nodeIt;

sem_t CronenbergController::MSG_QUEUE;
pthread_mutex_t CronenbergController::MUTEX_INCOMMING = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t CronenbergController::MUTEX_OUTPGOING = PTHREAD_MUTEX_INITIALIZER;

CronenbergController *CronenbergController::INSTANCE = NULL;

bool CronenbergController::VerifyUUID(uint8_t *recvUUID){
    uint8_t *thisUUID = GetUUID();
    for(uint8_t idx = 0; idx < RequestID::DEFAULT_UUID_SIZE; idx++){
        if(thisUUID[idx] != recvUUID[idx])
            return false;
    }

    return true;
}

void *CronenbergController::NodeRoutine(void *args){
    CronenbergPacket *packet;
    controller_t *arguments = (controller_t *)args;
    uint16_t recvLength;
    uint8_t *recvData;

    while(arguments->running){
		if ((arguments->incomming.size() == 0) && (arguments->outgoing.size() == 0)) {
			usleep(1000);
			continue;
		}
		//sem_timedwait(&CronenbergController::MSG_QUEUE, &semaphoreTimeout);
        if(arguments->incomming.size() > 0){
            pthread_mutex_lock(&CronenbergController::MUTEX_INCOMMING);
            packet = arguments->incomming[0];
            pthread_mutex_unlock(&CronenbergController::MUTEX_INCOMMING);

            PacketType type = packet->GetPacketType();
            if(type == PacketType::AckNack){
                AckNack *ackPayload = (AckNack *)packet->GetPayload();
                pthread_mutex_lock(&CronenbergController::MUTEX_OUTPGOING);
                outputIt it = arguments->outgoing.begin();
                while(it != arguments->outgoing.end()){
                    if((*it).first->GetPacketID() == ackPayload->GetPacketID()){
                        // remove this msg
                        delete (*it).first;
                        arguments->outgoing.erase(it);

                        // if waiting sync set it complete!
                        if(arguments->status == ControllerStatus::WaitingSync){
                            arguments->status = ControllerStatus::Running;
                        }
                        break; // no need to further search
                    }
                }
                pthread_mutex_unlock(&CronenbergController::MUTEX_OUTPGOING);
            }else if(type == PacketType::PingPong){
                PingPong *pingPayload = (PingPong *)packet->GetPayload();
                if(pingPayload->GetType() == PingPong::Type::Ping){
                    // no queueing, send the message right away
                    CronenbergPacket *pongResponse =
                        new CronenbergPacket(arguments->senderID, CronenbergController::BASE_ID);
                    pongResponse->AddPayload(new PingPong(PingPong::Type::Pong));

                    recvLength = pongResponse->GetLength();
                    recvData = new uint8_t[recvLength];
                    pongResponse->Parse(recvData, &recvLength);
					INSTANCE->SendData(packet->GetSender(), recvData, recvLength);

                    delete pongResponse;
                    delete[] recvData;
                }
            }else if(type == PacketType::ResponseID){
                ResponseID *respIDPayload = (ResponseID *)packet->GetPayload();
                if(INSTANCE->VerifyUUID(respIDPayload->GetUUID())){
                    // our sender ID
                    arguments->senderID = respIDPayload->GetReceivedID();
                    arguments->status = ControllerStatus::WaitingSync;
                    arguments->timestampOffset = INSTANCE->GetTimestamp();

                    // no queueing, send the message right away
                    CronenbergPacket *sync =
                        new CronenbergPacket(arguments->senderID, CronenbergController::BASE_ID);
                    sync->SetAsSync();

                    recvLength = sync->GetLength();
                    recvData = new uint8_t[recvLength];
                    sync->Parse(recvData, &recvLength);
                    INSTANCE->SendData(packet->GetSender(), recvData, recvLength);

                    pthread_mutex_lock(&CronenbergController::MUTEX_OUTPGOING);
                    arguments->outgoing.push_back(outPair(sync, INSTANCE->GetTimestamp()));
                    pthread_mutex_unlock(&CronenbergController::MUTEX_OUTPGOING);
                    delete[] recvData;
                }
            }

            // Clean-up
            pthread_mutex_lock(&CronenbergController::MUTEX_INCOMMING);
            inputIt it = arguments->incomming.begin();
            delete packet;
            arguments->incomming.erase(it);
            pthread_mutex_unlock(&CronenbergController::MUTEX_INCOMMING);
        }else if(arguments->outgoing.size() > 0){
            // thread sending
            pthread_mutex_lock(&CronenbergController::MUTEX_OUTPGOING);
			packet = INSTANCE->GetFirstExpired();
			pthread_mutex_unlock(&CronenbergController::MUTEX_OUTPGOING);
			if (packet == NULL) {
				usleep(1000);
				continue; // nothing to do
			}

            uint16_t sendLength = packet->GetLength();
            uint8_t *sendData = new uint8_t[sendLength];
            packet->Parse(sendData, &sendLength);
            INSTANCE->SendData(packet->GetSender(), sendData, sendLength);
            delete[] sendData;

            // remove if no need for ACK
            PacketType packetType = packet->GetType();
            if(packetType == PacketType::PingPong || packetType == PacketType::RequestID || packetType == PacketType::ResponseID){
                pthread_mutex_lock(&CronenbergController::MUTEX_OUTPGOING);
                outputIt it = arguments->outgoing.begin();
                delete packet;
                arguments->outgoing.erase(it);
                pthread_mutex_unlock(&CronenbergController::MUTEX_OUTPGOING);
            }
        }
    }

    return NULL;
}

void *CronenbergController::BaseRoutine(void *args){
	CronenbergPacket *packet;
	controller_t *arguments = (controller_t *)args;

	while (arguments->running) {
		if ((arguments->incomming.size() == 0) && (arguments->outgoing.size() == 0)) {
			usleep(1000);
			continue;
		}
		//sem_timedwait(&CronenbergController::MSG_QUEUE, &semaphoreTimeout);
        if(arguments->incomming.size() > 0){
            pthread_mutex_lock(&CronenbergController::MUTEX_INCOMMING);
			packet = arguments->incomming[0];
            pthread_mutex_unlock(&CronenbergController::MUTEX_INCOMMING);

            PacketType type = packet->GetPacketType();
            if(type == PacketType::AckNack){
                AckNack *ackPayload = (AckNack *)packet->GetPayload();
                pthread_mutex_lock(&CronenbergController::MUTEX_OUTPGOING);
                outputIt it = arguments->outgoing.begin();
                while(it != arguments->outgoing.end()){
                    if((*it).first->GetPacketID() == ackPayload->GetPacketID()){
                        // remove this msg
                        delete (*it).first;
                        arguments->outgoing.erase(it);
                        break; // no need to further search
                    }
                }
                pthread_mutex_unlock(&CronenbergController::MUTEX_OUTPGOING);
            }else if(type == PacketType::PingPong){
                PingPong *pingPayload = (PingPong *)packet->GetPayload();
                if(pingPayload->GetType() == PingPong::Type::Pong){
                    NodeInfo *senderNode = INSTANCE->GetNodeInfo(packet->GetSender());
                    if(senderNode != NULL){
                        senderNode->SetPong(INSTANCE->GetTimestamp());
                    }
                }
            }else if(type == PacketType::RequestID){
                RequestID *reqIDPayload = (RequestID *)packet->GetPayload();
                uint8_t respSenderID = INSTANCE->GetSenderIDByUUID(reqIDPayload->GetUUID());

                NodeInfo *senderNode = INSTANCE->GetNodeInfo(respSenderID);
                if(senderNode == NULL){
                    // add the node
                    arguments->nodes.insert(new NodeInfo(respSenderID));
                }

                CronenbergPacket *response =
                    new CronenbergPacket(arguments->senderID, packet->GetSender());
                ResponseID* resp = new ResponseID(reqIDPayload->GetUUID(), respSenderID);
                response->AddPayload(resp);

                pthread_mutex_lock(&CronenbergController::MUTEX_OUTPGOING);
                arguments->outgoing.push_back(outPair(response, INSTANCE->GetTimestamp()));
                pthread_mutex_unlock(&CronenbergController::MUTEX_OUTPGOING);
				sem_post(&CronenbergController::MSG_QUEUE);
            }else if(type == PacketType::Sync){
                NodeInfo *senderNode = INSTANCE->GetNodeInfo(packet->GetSender());
                if(senderNode != NULL){
                    senderNode->Resync();
                }
            }else if(type == PacketType::DataPacket){
                DataPacket *dataPkt = (DataPacket *)packet->GetPayload();
                vector<CronenbergData *> *dataArray =
                    new vector<CronenbergData *>(dataPkt->GetData());
                NodeInfo *senderNode = INSTANCE->GetNodeInfo(packet->GetSender());
                if(senderNode != NULL){
                    INSTANCE->PostData(dataArray, *senderNode);
                }
            }

            // Clean-up
            pthread_mutex_lock(&CronenbergController::MUTEX_INCOMMING);
            inputIt it = arguments->incomming.begin();
            delete packet;
            arguments->incomming.erase(it);
            pthread_mutex_unlock(&CronenbergController::MUTEX_INCOMMING);
        }else if(arguments->outgoing.size() > 0){
            // thread sending
			pthread_mutex_lock(&CronenbergController::MUTEX_OUTPGOING);
			packet = INSTANCE->GetFirstExpired();
			pthread_mutex_unlock(&CronenbergController::MUTEX_OUTPGOING);
			if (packet == NULL) {
				usleep(1000);
				continue; // nothing to do
			}

            uint16_t sendLength = packet->GetLength();
            uint8_t *sendData = new uint8_t[sendLength];
            packet->Parse(sendData, &sendLength);
            INSTANCE->SendData(packet->GetDestination(), sendData, sendLength);
			if (packet->GetPacketType() == PacketType::ResponseID) {
				ResponseID *payload = (ResponseID *)packet->GetPayload();
				INSTANCE->UpdateSenderID(packet->GetSender(), payload->GetReceivedID());
			}

            delete[] sendData;

            // remove if no need for ACK
            PacketType packetType = packet->GetType();
            if(packetType == PacketType::PingPong || packetType == PacketType::RequestID || packetType == PacketType::ResponseID){
                pthread_mutex_lock(&CronenbergController::MUTEX_OUTPGOING);
                outputIt it = arguments->outgoing.begin();
                delete packet;
                arguments->outgoing.erase(it);
                pthread_mutex_unlock(&CronenbergController::MUTEX_OUTPGOING);
            }
        }
    }

    return NULL;
}

void CronenbergController::ClearBuffers(void){
    inputIt it = m_threadArgs.incomming.begin();
    while(it != m_threadArgs.incomming.end())
        delete (*it);
    m_threadArgs.incomming.clear();

    outputIt oIt = m_threadArgs.outgoing.begin();
    while(oIt != m_threadArgs.outgoing.end())
        delete (*oIt).first;
    m_threadArgs.outgoing.clear();
}

NodeInfo *CronenbergController::GetNodeInfo(uint8_t senderID){
    nodeIt nInfo = m_threadArgs.nodes.begin();
	while (nInfo != m_threadArgs.nodes.end()) {
		if ((*nInfo)->GetSenderID() == senderID)
			return (*nInfo);
	}

	return NULL;
}

CronenbergPacket *CronenbergController::GetFirstExpired(void) {
	uint32_t now = GetTimestamp();
	outputIt it = m_threadArgs.outgoing.begin();
	while (it != m_threadArgs.outgoing.end()) {
		if ((*it).second < now) {
			(*it).second = now + CronenbergController::TIMEOUT;
			return (*it).first;
		}

		it++;
	}

	return NULL;
}

CronenbergController::CronenbergController(void){
    m_threadArgs.packetID = 0;
    m_threadArgs.senderID = 0xFF;
    m_threadArgs.status = ControllerStatus::NotInitiated;
    m_threadArgs.timestampOffset = 0;
    m_threadArgs.msToTransfer = 0;
    m_threadArgs.pingTime = 0;
    m_threadArgs.running = false;
}

CronenbergController::~CronenbergController(void){
    m_threadArgs.running = false;
    ClearBuffers();
    sem_post(&CronenbergController::MSG_QUEUE);

    pthread_join(m_runnerThread, NULL);
}

CronenbergController *CronenbergController::GetInstance(void){
    if(CronenbergController::INSTANCE == NULL){
        int result = sem_init(&CronenbergController::MSG_QUEUE, 0, 0);
        if(result >= 0){
            CronenbergController::INSTANCE = new CronenbergController();
        }
    }
    
    return CronenbergController::INSTANCE;
}

void CronenbergController::Dispose(void){
    delete CronenbergController::INSTANCE;

    CronenbergController::INSTANCE = NULL;
}

bool CronenbergController::RunNode(void){
    CronenbergPacket *packet;
    packet = new CronenbergPacket(m_threadArgs.senderID, CronenbergController::BASE_ID);
    packet->AddPayload(new RequestID(GetUUID()));

    m_threadArgs.status = ControllerStatus::WaitingSenderID;
    pthread_mutex_lock(&CronenbergController::MUTEX_OUTPGOING);
    m_threadArgs.outgoing.push_back(outPair(packet, GetTimestamp()));
    pthread_mutex_unlock(&CronenbergController::MUTEX_OUTPGOING);
    sem_post(&CronenbergController::MSG_QUEUE);

    m_threadArgs.running = true;
    int result = pthread_create(&m_runnerThread, NULL, NodeRoutine, (void *)&m_threadArgs);
    if(result < 0){
        m_threadArgs.running = false;
        return false;
    }
    return true;
}

bool CronenbergController::RunBase(void){
    m_threadArgs.senderID = CronenbergController::BASE_ID;
    m_threadArgs.status = ControllerStatus::Running;

    m_threadArgs.running = true;
    int result = pthread_create(&m_runnerThread, NULL, BaseRoutine, (void *)&m_threadArgs);
    if(result < 0){
        m_threadArgs.running = false;
        return false;
    }
    return true;
}

bool CronenbergController::ReceiveData(uint8_t *data, uint16_t size){
    if(!m_threadArgs.running)
        return false;

    CronenbergPacket *packet = new CronenbergPacket(data, size);
    if(packet->GetDestination() != m_threadArgs.senderID){
        return false;
    }

    uint8_t destination = packet->GetSender();
    uint8_t packetID = packet->GetPacketID();
    PacketType packetType = packet->GetType();
    if(packet->IsValid()){
		pthread_mutex_lock(&CronenbergController::MUTEX_INCOMMING);
        m_threadArgs.incomming.push_back(packet);
		pthread_mutex_unlock(&CronenbergController::MUTEX_INCOMMING);
		sem_post(&CronenbergController::MSG_QUEUE);

        if(packetType == PacketType::DataPacket || packetType == PacketType::PacketArray || packetType == PacketType::Sync){
            packet = new CronenbergPacket(m_threadArgs.senderID, destination);
            packet->AddPayload(new AckNack(AckNack::Result::ACK, m_threadArgs.senderID, packetID));

            pthread_mutex_lock(&CronenbergController::MUTEX_OUTPGOING);
            m_threadArgs.outgoing.push_back(outPair(packet, GetTimestamp()));
            pthread_mutex_unlock(&CronenbergController::MUTEX_OUTPGOING);
            sem_post(&CronenbergController::MSG_QUEUE);
        }
    }

    return true;
}

uint8_t CronenbergController::GetID(void){
    return m_threadArgs.senderID;
}

uint32_t CronenbergController::GetCurrentTimestamp(void){
    return GetTimestamp() - m_threadArgs.timestampOffset;
}

uint8_t CronenbergController::GetNextPacketID(void){
    return m_threadArgs.packetID++;
}

bool CronenbergController::SendDataPacket(vector<CronenbergData *> data){
    if(m_threadArgs.running){

    }

    return false; // controller not running
}
