#include <stdlib.h>
#include "PacketPayload.h"
#include "PacketType.h"
#include "DataPacket.h"

using namespace cronenberg;

PacketPayload *PacketPayload::Copy(const PacketPayload &copy){
    switch(copy.GetType()){
        case PacketType::DataPacket:
            return new DataPacket((DataPacket &)copy);
        case PacketType::ACK:
        case PacketType::NACK:
        case PacketType::Ping:
        case PacketType::Pong:
        case PacketType::PacketArray:
        case PacketType::Sync:
            return NULL;
    }
}

