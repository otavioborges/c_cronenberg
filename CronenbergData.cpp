#include <string.h>
#include "CronenbergData.h"

using namespace cronenberg;

void CronenbergData::DataToArray(uint8_t *data, uint16_t *length){
	if (m_dataFormat == DataType::Booleans || m_dataFormat == DataType::UsignedShort
		|| m_dataFormat == DataType::SignedShort){
		memcpy(data, (uint8_t *)&m_value, 1);
		*length += 1;
	}else if (m_dataFormat == DataType::UnsginedInteger
		|| m_dataFormat == DataType::SignedInteger){
		memcpy(data, (uint8_t *)&m_value, 2);
		*length += 2;
	}else{
		memcpy(data, (uint8_t *)&m_value, 4);
		*length += 4;
	}
}

CronenbergData::CronenbergData(uint16_t nodeID, uint16_t timestamp, DataType format, void *value) {
	m_nodeID = nodeID;
	m_timestamp = timestamp;
	m_dataFormat = format;

	m_value = *((uint32_t *)value);
}

uint16_t CronenbergData::GetNodeID(void) {
	return m_nodeID;
}

uint16_t CronenbergData::GetTimestamp(void) {
	return m_timestamp;
}

DataType CronenbergData::GetDataFormat(void) {
	return m_dataFormat;
}

uint16_t CronenbergData::Length(void) {
	uint16_t overhead = 5;
	if (m_dataFormat == DataType::Booleans || m_dataFormat == DataType::UsignedShort
		|| m_dataFormat == DataType::SignedShort)
		overhead += 1;
	else if (m_dataFormat == DataType::UnsginedInteger || m_dataFormat == DataType::SignedInteger)
		overhead += 2;
	else
		overhead += 4;

	return overhead;
}

void CronenbergData::Parse(uint8_t *data, uint16_t *resultLength) {
	*resultLength = 5;
	data[0] = m_nodeID;
	data[1] = (m_nodeID >> 8);
	data[2] = m_timestamp;
	data[3] = (m_timestamp >> 8);
	data[4] = (uint8_t)m_dataFormat;
	DataToArray((data + 5), resultLength);
}