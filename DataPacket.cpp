#include <string.h>
#include "DataPacket.h"

using namespace cronenberg;
using namespace std;

DataPacket::DataPacket(vector<CronenbergData *> data) {
	m_data = data;
}

DataPacket::DataPacket(uint8_t *data, uint16_t length){
	uint16_t offset = 0;
	uint16_t nodeID = 0;
	uint16_t timestamp = 0;
	DataType format;
	while(offset < length){
		memcpy(&nodeID, (data + offset), 2);
		memcpy(&timestamp, (data + offset + 2), 2);
		format = (DataType)data[4 + offset];
		m_data.push_back(new CronenbergData(nodeID, timestamp, format, (void *)(data + offset + 5)));
		
		if(format == DataType::Booleans || format == DataType::UsignedShort
			|| format == DataType::SignedShort)
			offset += 6;
		else if(format == DataType::UnsginedInteger || format == DataType::SignedInteger)
			offset += 7;
		else
			offset += 9;
	}
}

DataPacket::~DataPacket(void){
	vector<CronenbergData *>::iterator it = m_data.begin();
	while(it != m_data.end()){
		delete (*it);
		it++;
	}

	m_data.clear();
}

void DataPacket::AddData(CronenbergData *data) {
	m_data.push_back(data);
}

uint16_t DataPacket::Length(void) {
	uint16_t size = 0;
	vector<CronenbergData *>::iterator it;
	for (it = m_data.begin(); it != m_data.end(); ++it)
		size += (*it)->Length();

	return size;
}

void DataPacket::Parse(uint8_t *data, uint16_t *resultLength) {
	uint16_t offset = 0;

	vector<CronenbergData *>::iterator it;
	for (it = m_data.begin(); it != m_data.end(); ++it) {
		uint16_t entryLength = 0;

		(*it)->Parse((data+offset), &entryLength);
		offset += entryLength;
	}

	*resultLength = Length();
}

PacketType DataPacket::GetType(void) const{
	return PacketType::DataPacket;
}
