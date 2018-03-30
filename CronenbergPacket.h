#ifndef CRONENBERGPACKET_H_
#define CRONENBERGPACKET_H_

#include <stdint.h>
#include "cronenberg.h"
#include "PacketType.h"
#include "PacketPayload.h"

class cronenberg::CronenbergPacket {
	private:
		static const uint8_t HEADER = 0xFB;
		static const uint8_t FOOTER = 0xFA;

		uint8_t m_length;
		uint32_t m_timestamp;
		cronenberg::PacketType m_type;
		uint8_t m_sender;
		uint8_t m_destination;
		uint8_t m_packetID;
		cronenberg::PacketPayload &m_payload;
		uint16_t m_checksum;

		uint16_t CalculateChecksum(void);
	public:
		CronenbergPacket(uint8_t *data, uint16_t length);
		CronenbergPacket(uint8_t sender, uint8_t destination, uint8_t packetID, cronenberg::PacketPayload &payload);
};

#endif /* CRONENBERGPACKET_H_ */
