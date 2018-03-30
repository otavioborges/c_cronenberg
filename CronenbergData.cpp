#include "CronenbergData.h"

using namespace cronenberg;

CronenbergData::CronenbergData(uint16_t nodeID, uint16_t timestamp, DataType format, void *value) {
	m_nodeID = nodeID;
	m_timestamp = timestamp;
	m_dataFormat = format;

	switch (m_dataFormat) {
		case DataType::Booleans:
			m_booleanValue = *((uint8_t *)value);
			break;
		case DataType::UsignedShort:
			m_uShortvalue = *((uint8_t *)value);
			break;
		case DataType::SignedShort:
			m_ShortValue = *((int8_t *)value);
			break;
		case DataType::UnsginedInteger:
			m_uIntValue = *((uint16_t *)value);
			break;
		case DataType::SignedInteger:
			m_intValue = *((int16_t *)value);
			break;
		case DataType::UnsignedLong:
			m_uLongValue = *((uint32_t *)value);
			break;
		case DataType::SignedLong:
			m_longValue = *((int32_t *)value);
			break;
		case DataType::Float:
			m_floatValue = *((float *)value);
			break;
	}
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
	else if (m_dataFormat == DataType::UnsginedInteger || m_dataFormat == DataType::SignedShort)
		overhead += 2;
	else
		overhead += 4;

	return overhead;
}

void CronenbergData::Parse(uint8_t *data, uint16_t *resultLength) {
	data = new uint8_t[5];

	data[0] = m_nodeID;
	data[1] = (m_nodeID >> 8);
	data[2] = m_timestamp;
	data[3] = (m_timestamp >> 8);
	data[4] = (uint8_t)m_dataFormat;

	*resultLength = 5;
}