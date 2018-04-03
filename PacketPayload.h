#ifndef PACKETPAYLOAD_H_
#define PACKETPAYLOAD_H_

#include <stdint.h>
#include "cronenberg.h"

class cronenberg::PacketPayload {
	public:
		static PacketPayload* ReceiveFromArray(uint8_t *data, uint16_t length, cronenberg::PacketType type);
		static PacketPayload* Receive(uint8_t *data, uint16_t length) { return NULL; };
		virtual ~PacketPayload(void) {};
		virtual uint16_t Length(void) = 0;
		virtual void Parse(uint8_t *data, uint16_t *resultLength) = 0;
		virtual cronenberg::PacketType GetType(void) const = 0;
		static PacketPayload *Copy(const PacketPayload &copy);
};

#endif /* PACKETPAYLOAD_H_ */
