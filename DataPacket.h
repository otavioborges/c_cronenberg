#ifndef DATAPACKET_H_
#define DATAPACKET_H_

#include <vector>
#include <stdint.h>
#include "cronenberg.h"
#include "PacketPayload.h"
#include "PacketType.h"
#include "CronenbergData.h"

class cronenberg::DataPacket : public cronenberg::PacketPayload {
	private:
		std::vector<CronenbergData> m_data;
	public:
		static const PacketType TYPE;

		DataPacket(void) {}
		DataPacket(std::vector<CronenbergData> data);
		void AddData(CronenbergData data);
		uint16_t Size(void);
		void Parse(uint8_t *data, uint16_t *resultLength);
};

#endif /* DATAPACKET_H_ */
