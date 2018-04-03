#ifndef CRONENBERCONTROLLER_H_
#define CRONENBERCONTROLLER_H_

#include <stdint.h>
#include <vector>
#include <set>
#include <pthread.h>
#include <semaphore.h>
#include "cronenberg.h"
#include "ControllerStatus.h"
#include "CronenbergPacket.h"
#include "NodeInfo.h"

// TODO: singleton
class cronenberg::CronenbergController{
    private:
        static const uint8_t BASE_ID = 0;
        
        static sem_t MSG_QUEUE;
        static pthread_mutex_t MUTEX_INCOMMING;
        static pthread_mutex_t MUTEX_OUTPGOING;

        static CronenbergController *INSTANCE;
        typedef struct{
            uint8_t packetID;
            uint8_t senderID;
            uint32_t timestampOffset;
            uint32_t msToTransfer;
            uint32_t pingTime;
            cronenberg::ControllerStatus status;
            std::vector<CronenbergPacket *> incomming;
            std::vector<CronenbergPacket *> outgoing;
            std::set<NodeInfo *> nodes;
            bool running;
        } controller_t;

        controller_t m_threadArgs;
        pthread_t m_runnerThread;

        CronenbergController(void);
        bool VerifyUUID(uint8_t *recvUUID);

        static void *NodeRoutine(void *args);
        static void *BaseRoutine(void *args);
        void ClearBuffers(void);
        cronenberg::NodeInfo *GetNodeInfo(uint8_t senderID);
    public:
        
        ~CronenbergController(void);
        static CronenbergController *GetInstance(void);
        static void Dispose(void);

        bool RunNode(void);
        bool RunBase(void);
        
        bool ReceiveData(uint8_t *data, uint16_t size);
        uint8_t *GetUUID(void);                                              // must be overloaded
        void SendData(uint8_t *data, uint16_t size);                         // must be overloaded
        uint32_t GetTimestamp(void);                                         // must be overloaded
        uint8_t GetSenderIDByUUID(uint8_t *uuid);                            // must be overloaded
        void PostData(std::vector<CronenbergData *> *data, NodeInfo sender); // must be overloaded
        uint32_t GetCurrentTimestamp(void);
        uint8_t GetNextPacketID(void);

        // Send defined Packets
        bool SendDataPacket(std::vector<CronenbergData *> data);
};

#endif /* CRONENBERCONTROLLER_H_ */