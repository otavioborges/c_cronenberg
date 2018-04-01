#include <stdlib.h>
#include "CronenbergController.h"

using namespace cronenberg;

CronenbergController *CronenbergController::INSTANCE = NULL;

CronenbergController::CronenbergController(void){
    m_packetID = 0;
}

CronenbergController::~CronenbergController(void){
    delete CronenbergController::INSTANCE;
}

CronenbergController *CronenbergController::GetInstance(void){
    if(CronenbergController::INSTANCE == NULL)
        CronenbergController::INSTANCE = new CronenbergController();
    
    return CronenbergController::INSTANCE;
}

uint8_t CronenbergController::GetNextPacketID(void){
    return m_packetID++;
}