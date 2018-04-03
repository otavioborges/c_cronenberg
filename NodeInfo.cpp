#include "NodeInfo.h"

using namespace std;
using namespace boost;
using namespace cronenberg;

tm m_syncTime;
uint32_t m_ping;
uint8_t m_senderID;

NodeInfo::NodeInfo(uint8_t senderID){
    m_syncPoint = posix_time::microsec_clock::universal_time();
    m_senderID = senderID;
}

void NodeInfo::Resync(void){
    m_syncPoint = posix_time::microsec_clock::universal_time();
}

void NodeInfo::SetPing(uint32_t ms){
    m_timestampOnPing = ms;
}

void NodeInfo::SetPong(uint32_t ms){
    m_ping = ((ms - m_timestampOnPing) / 2);
}

uint8_t NodeInfo::GetSenderID(void){
    return m_senderID;
}

uint32_t NodeInfo::GetCurrentPing(void){
    return m_ping;
}

posix_time::ptime NodeInfo::ShiftTimestamp(uint32_t timestamp){
    return (m_syncPoint + posix_time::millisec(timestamp));
}

string NodeInfo::ToString(uint32_t timestamp){
    posix_time::ptime shiftedTime = ShiftTimestamp(timestamp);
    string rtnValue;
    istringstream dateTime(rtnValue);
    posix_time::time_facet *format = new posix_time::time_facet("%Y-%m-%d %H:%M:%S.%3 %Z");
    dateTime.imbue(std::locale(std::locale("UTC"), format));

    dateTime >> shiftedTime;
    return rtnValue;
}
