#include <string.h>
#include "DataPacket.h"

using namespace cronenberg;
using namespace std;

const PacketType DataPacket::TYPE = PacketType::DataType;

DataPacket::DataPacket(vector<CronenbergData> data) {
	m_data = data;
}

void DataPacket::AddData(CronenbergData data) {
	m_data.push_back(data);
}

uint16_t DataPacket::Size(void) {
	uint16_t size = 0;
	vector<CronenbergData>::iterator it;
	for (it = m_data.begin(); it != m_data.end(); ++it)
		size += (*it).Length();

	return size;
}

void DataPacket::Parse(uint8_t *data, uint16_t *resultLength) {
	uint16_t length = Size();
	uint16_t offset = 0;
	data = new uint8_t[length];

	vector<CronenbergData>::iterator it;
	for (it = m_data.begin(); it != m_data.end(); ++it) {
		uint8_t *entryData;
		uint16_t entryLength;

		(*it).Parse(entryData, &entryLength);
		memcpy((data + offset), entryData, entryLength);
		offset += entryLength;
	}

	*resultLength = length;
}