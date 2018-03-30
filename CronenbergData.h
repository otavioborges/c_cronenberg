#ifndef CRONENBERGDATA_H_
#define CRONENBERGDATA_H_

#include <stdint.h>
#include "cronenberg.h"
#include "DataType.h"

class cronenberg::CronenbergData{
	private:
		uint8_t m_booleanValue;
		uint8_t m_uShortvalue;
		int8_t m_ShortValue;
		uint16_t m_uIntValue;
		int16_t m_intValue;
		uint32_t m_uLongValue;
		int32_t m_longValue;
		float m_floatValue;

		uint16_t m_nodeID;
		uint16_t m_timestamp;
		cronenberg::DataType m_dataFormat;

	public:
		CronenbergData(uint16_t nodeID, uint16_t timestamp, cronenberg::DataType format, void *value);

		uint16_t GetNodeID(void);
		uint16_t GetTimestamp(void);
		cronenberg::DataType GetDataFormat(void);
		uint16_t Length(void);
		void Parse(uint8_t *data, uint16_t *resultLength);
};

#endif /* CRONENBERGDATA_H_ */
