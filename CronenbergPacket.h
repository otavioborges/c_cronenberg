#ifndef CRONENBERGPACKET_H_
#define CRONENBERGPACKET_H_

#include <stdint.h>
#include <string>
#include "cronenberg.h"
#include "PacketType.h"
#include "PacketPayload.h"

class cronenberg::CronenbergPacket {
	private:
		static const uint8_t HEADER = 0xFB;
		static const uint8_t FOOTER = 0xFA;
		static const uint16_t OVERHEAD = 13u;

		uint32_t m_timestamp;
		cronenberg::PacketType m_type;
		uint8_t m_sender;
		uint8_t m_destination;
		uint8_t m_packetID;
		cronenberg::PacketPayload *m_payload;
		bool m_isValid;

		uint16_t CalculateChecksum(uint8_t *data, uint16_t length);
	public:
		CronenbergPacket(uint8_t *data, uint16_t length);
		CronenbergPacket(uint8_t sender, uint8_t destination);
		~CronenbergPacket(void);

		void AddPayload(cronenberg::PacketPayload *payload);

		// GEt methods
		uint16_t GetLength(void);
		uint16_t GetTimeDiff(void);
		uint32_t GetTimestamp(void);
		bool IsValid(void);

		cronenberg::PacketType GetType(void);
		cronenberg::PacketPayload *GetPayload(void);

		bool Parse(uint8_t *data, uint16_t *returnedLength);
		std::string ToString(void);
};

#endif /* CRONENBERGPACKET_H_ */
