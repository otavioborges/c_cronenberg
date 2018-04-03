#ifndef NODEINFO_H_
#define NODEINFO_H_

#include <stdint.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
#include "cronenberg.h"

class cronenberg::NodeInfo{
    private:
        boost::posix_time::ptime m_syncPoint;
        uint32_t m_ping;
        uint32_t m_timestampOnPing;
        uint8_t m_senderID;

    public:
        NodeInfo(uint8_t senderID);
        void Resync(void);
        void SetPing(uint32_t ms);
        void SetPong(uint32_t ms);

        uint8_t GetSenderID(void);
        uint32_t GetCurrentPing(void);
        boost::posix_time::ptime ShiftTimestamp(uint32_t timestamp);
        std::string ToString(uint32_t timestamp);
};

#endif /* NODEINFO_H_ */
