#include <string.h>
#include <stdlib.h>
#include "CronenbergPacket.h"
#include "CronenbergController.h"

// Packet Types
#include "DataPacket.h"

using namespace cronenberg;
using namespace std;

uint16_t CronenbergPacket::CalculateChecksum(uint8_t *data, uint16_t length){
	uint16_t result = 0xFFFF;

	for(uint16_t idx = 0; idx < length; idx++)
		result ^= *((uint16_t *)(data + idx));

	result = 0x7FFF;
	result = ~(result);
	return result;
}

CronenbergPacket::CronenbergPacket(uint8_t *data, uint16_t length) {
	if(data[0] != CronenbergPacket::HEADER){
		m_isValid = false;
		return;
	}

	uint16_t packetSize = data[1] + 2;
	if(length != packetSize){
		m_isValid = false;
		return;
	}

	if(data[packetSize - 1] != CronenbergPacket::FOOTER){
		m_isValid = false;
		return;
	}

	uint16_t calcCRC = CalculateChecksum(data, (packetSize -3));
	uint16_t recvCRC = data[packetSize - 2];
	recvCRC = (recvCRC << 8) + data[packetSize - 3];

	if(calcCRC != recvCRC){
		m_isValid = false;
		return;
	}

	// packet is valid! yey o/
	m_isValid = true;

	memcpy(&m_timestamp, (uint32_t *)(data+2), 4);
	m_type = (PacketType)data[6];
	m_sender = data[7];
	m_destination = data[8];
	m_packetID = data[9];

	uint16_t payloadSize = packetSize - CronenbergPacket::OVERHEAD;
	if(payloadSize == 0){
		m_payload = NULL;
		// if its sync no problem
		if(m_type == PacketType::Sync){

			m_isValid = true;
		}else{
			m_isValid = false; // shit happened
		}
	}else{
		m_payload = PacketPayload::ReceiveFromArray((data+10), payloadSize, m_type);
	}
}

CronenbergPacket::CronenbergPacket(uint8_t sender, uint8_t destination) {
		m_timestamp = CronenbergController::GetInstance()->GetCurrentTimestamp();
		m_sender = sender;
		m_destination = destination;
		m_packetID = CronenbergController::GetInstance()->GetNextPacketID();
		m_isValid = false;
}

CronenbergPacket::~CronenbergPacket(void){
	delete m_payload;
}

void CronenbergPacket::AddPayload(cronenberg::PacketPayload *payload){
	m_type = payload->GetType();
	m_payload = PacketPayload::Copy(*payload);

	m_isValid = true;
}

void CronenbergPacket::SetAsSync(void){
	m_type = PacketType::Sync;
	m_payload = NULL;

	m_isValid = true;
}

PacketType CronenbergPacket::GetPacketType(void){
	return m_type;
}

uint8_t CronenbergPacket::GetSender(void){
	return m_sender;
}

uint8_t CronenbergPacket::GetDestination(void){
	return m_destination;
}

uint8_t CronenbergPacket::GetPacketID(void){
	return m_packetID;
}

uint16_t CronenbergPacket::GetLength(void){
	return CronenbergPacket::OVERHEAD + m_payload->Length();
}

uint16_t CronenbergPacket::GetTimeDiff(void){
	uint32_t timeNow = CronenbergController::GetInstance()->GetCurrentTimestamp();
	return (uint16_t)(timeNow - m_timestamp);
}

uint32_t CronenbergPacket::GetTimestamp(void){
	return m_timestamp;
}

bool CronenbergPacket::IsValid(void){
	return m_isValid;
}

PacketType CronenbergPacket::GetType(void){
	return m_type;
}

PacketPayload *CronenbergPacket::GetPayload(void){
	return m_payload;
}

bool CronenbergPacket::Parse(uint8_t *data, uint16_t *returnedLength){
	if(!m_isValid)
		return false;
	
	uint16_t offset = 0;
	uint16_t payloadLength = 0;

	data[offset++] = CronenbergPacket::HEADER;
	data[offset++] = GetLength() - 2;
	memcpy((data + offset), (uint8_t *)&m_timestamp, 4);
	offset = 6;
	data[offset++] = (uint8_t)m_type;
	data[offset++] = m_sender;
	data[offset++] = m_destination;
	data[offset++] = m_packetID;
	if(m_payload != NULL){
		m_payload->Parse((data+offset), &payloadLength);
		offset += payloadLength;
	}else{
		offset++; // no payload!
	}
	uint16_t crc = CalculateChecksum(data, offset);
	memcpy((data+offset), (uint8_t *)&crc, 2);
	offset += 2;
	data[offset++] = CronenbergPacket::FOOTER;

	*returnedLength = offset;

	return true;
}

string CronenbergPacket::ToString(void){
	if(!m_isValid)
		return "";

	uint8_t* check = new uint8_t[GetLength()];
	uint16_t size = 0;
	Parse(check, &size);

	char aux[7];
	string rtnValue = "{";
	for(uint16_t idx = 0; idx < size; idx++){
		sprintf(aux, "0x%02X, ", check[idx]);
		rtnValue += aux;
	}
	rtnValue = rtnValue.substr(0, rtnValue.length()-2);
	rtnValue += "}";

	delete[] check;
	return rtnValue;
}
