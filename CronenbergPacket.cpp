#include "CronenbergPacket.h"

// Packet Types
#include "DataPacket.h"

using namespace cronenberg;

uint16_t CronenbergPacket::CalculateChecksum(void) {
	return 0;
}

CronenbergPacket::CronenbergPacket(uint8_t *data, uint16_t length) {

}

CronenbergPacket::CronenbergPacket(uint8_t sender, uint8_t destination, uint8_t packetID, cronenberg::PacketPayload &payload) {

	switch (payload.TYPE) {
		case PacketType::DataType:
			m_payload = (DataPacket)payload;
			break;
	}
}
