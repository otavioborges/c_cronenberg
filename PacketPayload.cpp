#include <stdlib.h>
#include "PacketPayload.h"
#include "PacketType.h"
#include "DataPacket.h"
#include "AckNack.h"
#include "PingPong.h"
#include "RequestID.h"
#include "ResponseID.h"

using namespace cronenberg;

PacketPayload* PacketPayload::ReceiveFromArray(uint8_t *data, uint16_t length, cronenberg::PacketType type) {
	switch (type) {
		case cronenberg::PacketType::DataPacket:
			return DataPacket::Receive(data, length);
			break;
		case cronenberg::PacketType::AckNack:
			return AckNack::Receive(data, length);
			break;
		case cronenberg::PacketType::PingPong:
			return PingPong::Receive(data, length);
			break;
		case cronenberg::PacketType::PacketArray:
			return NULL;
			break;
		case cronenberg::PacketType::Sync:
			return NULL;
			break;
		case cronenberg::PacketType::RequestID:
			return RequestID::Receive(data, length);
			break;
		case cronenberg::PacketType::ResponseID:
			return ResponseID::Receive(data, length);
			break;
		default:
			return NULL;
			break;
	}
}

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

