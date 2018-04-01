#ifndef CRONENBERCONTROLLER_H_
#define CRONENBERCONTROLLER_H_

#include <stdint.h>
#include "cronenberg.h"

// TODO: singleton
class cronenberg::CronenbergController{
    private:
        static CronenbergController *INSTANCE;
        uint8_t m_packetID;
        CronenbergController(void);
    public:
        ~CronenbergController(void);
        static CronenbergController *GetInstance(void);

        uint32_t GetCurrentTimestamp(void);
        uint8_t GetNextPacketID(void);
};

#endif /* CRONENBERCONTROLLER_H_ */