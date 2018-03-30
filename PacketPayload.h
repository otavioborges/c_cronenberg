#ifndef PACKETPAYLOAD_H_
#define PACKETPAYLOAD_H_

#include <stdint.h>
#include "cronenberg.h"

class cronenberg::PacketPayload {
	public:
		static const PacketType TYPE;
		virtual uint16_t Size(void) = 0;
		virtual void Parse(uint8_t *data, uint16_t *resultLength) = 0;
};

#endif /* PACKETPAYLOAD_H_ */
