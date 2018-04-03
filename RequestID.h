#ifndef REQUESTID_H_
#define REQUESTID_H_

#include <stdint.h>
#include <stdbool.h>
#include "cronenberg.h"
#include "PacketType.h"
#include "PacketPayload.h"

class cronenberg::RequestID : public cronenberg::PacketPayload {
	private:
		uint8_t m_uuid[25];
	public:
		static const uint8_t DEFAULT_UUID_SIZE = 25;
		RequestID(uint8_t *uuid);
		RequestID* Receive(uint8_t *data, uint16_t length);
		~RequestID(void);
		uint16_t Length(void);
		void Parse(uint8_t *data, uint16_t *resultLength);
		cronenberg::PacketType GetType(void) const;

		// Get Methods
		uint8_t *GetUUID(void);
};

#endif /* REQUESTID_H_ */
