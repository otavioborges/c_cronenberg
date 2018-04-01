#ifndef CRONENBERGDATA_H_
#define CRONENBERGDATA_H_

#include <stdint.h>
#include "cronenberg.h"
#include "DataType.h"

class cronenberg::CronenbergData{
	private:
		uint32_t m_value;

		uint16_t m_nodeID;
		uint16_t m_timestamp;
		cronenberg::DataType m_dataFormat;

		void DataToArray(uint8_t *data, uint16_t *length);
	public:
		CronenbergData(uint16_t nodeID, uint16_t timestamp, cronenberg::DataType format, void *value);

		uint16_t GetNodeID(void);
		uint16_t GetTimestamp(void);
		cronenberg::DataType GetDataFormat(void);
		uint16_t Length(void);
		void Parse(uint8_t *data, uint16_t *resultLength);
};

#endif /* CRONENBERGDATA_H_ */
