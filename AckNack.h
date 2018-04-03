#ifndef ACKNACK_H_
#define ACKNACK_H_

#include <stdint.h>
#include <stdbool.h>
#include "cronenberg.h"
#include "PacketType.h"
#include "PacketPayload.h"

class cronenberg::AckNack : public cronenberg::PacketPayload {
	private:
		static const uint16_t ACK_NACK_SIZE = 3;

		uint8_t m_ack;
		uint8_t m_sender;
		uint8_t m_packetID;
	public:
		enum class Result{
			ACK = 0x01,
			NACK = 0x00
		};
		AckNack(AckNack::Result result, uint8_t sender, uint8_t packetID):
			m_ack((uint8_t)result), m_sender(sender), m_packetID(packetID) {};
		static AckNack* Receive(uint8_t *data, uint16_t length);
		~AckNack(void);
		uint16_t Length(void);
		void Parse(uint8_t *data, uint16_t *resultLength);
		cronenberg::PacketType GetType(void) const;

		// Get Methods
		AckNack::Result GetResult(void);
		uint8_t GetSender(void);
		uint8_t GetPacketID(void);
};

#endif /* ACKNACK_H_ */
