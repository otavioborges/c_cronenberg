#ifndef PACKETTYPE_H_
#define PACKETTYPE_H_

#include "cronenberg.h"

enum class cronenberg::PacketType {
	DataPacket	= 0x01,
	AckNack		= 0x02,
	PingPong	= 0x03,
	PacketArray	= 0x04,
	Sync		= 0x05,
	RequestID	= 0x06,
	ResponseID  = 0x07
};

#endif /* PACKETTYPE_H_ */
