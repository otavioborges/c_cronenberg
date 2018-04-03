#include <stdlib.h>
#include "PacketPayload.h"
#include "PacketType.h"
#include "DataPacket.h"
#include "AckNack.h"
#include "PingPong.h"
#include "RequestID.h"
#include "ResponseID.h"

using namespace cronenberg;

PacketPayload *PacketPayload::Copy(const PacketPayload &copy){
    switch(copy.GetType()){
        case PacketType::DataPacket:
            return new DataPacket((DataPacket &)copy);
        case PacketType::AckNack:
            return new AckNack((AckNack &)copy);
        case PacketType::PingPong:
            return new PingPong((PingPong &)copy);
        case PacketType::RequestID:
            return new RequestID((RequestID &)copy);
        case PacketType::ResponseID:
            return new ResponseID((ResponseID &)copy);
        case PacketType::PacketArray:
        default:
            return NULL;
    }
}

