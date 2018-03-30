#ifndef PACKETTYPE_H_
#define PACKETTYPE_H_

#include "cronenberg.h"

enum class cronenberg::PacketType {
	DataType	= 0x01,
	ACK			= 0x02,
	NACK		= 0x03,
	Ping		= 0x04,
	Pong		= 0x05,
	PacketArray	= 0x06,
	Sync		= 0x07
};

#endif /* PACKETTYPE_H_ */
