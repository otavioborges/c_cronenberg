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
		std::vector<CronenbergData *> m_data;
	public:
		DataPacket(void) {};
		DataPacket(std::vector<CronenbergData *> data);
		DataPacket(uint8_t *data, uint16_t length);
		PacketPayload* Copy(const PacketPayload &copy) const;
		~DataPacket(void);
		void AddData(CronenbergData *data);
		uint16_t Length(void);
		void Parse(uint8_t *data, uint16_t *resultLength);
		cronenberg::PacketType GetType(void) const;
};

#endif /* DATAPACKET_H_ */
