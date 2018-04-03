#ifndef RESPONSEID_H_
#define RESPONSEID_H_

#include <stdint.h>
#include <stdbool.h>
#include "cronenberg.h"
#include "PacketType.h"
#include "PacketPayload.h"

class cronenberg::ResponseID : public cronenberg::PacketPayload {
	private:
		uint8_t m_recvSenderID;
		uint8_t m_uuid[25];
	public:
		static const uint8_t DEFAULT_UUID_SIZE = 25;
		ResponseID(uint8_t *uuid, uint8_t senderID);
		static ResponseID* Receive(uint8_t *data, uint16_t length);
		~ResponseID(void);
		uint16_t Length(void);
		void Parse(uint8_t *data, uint16_t *resultLength);
		cronenberg::PacketType GetType(void) const;

		// Get Methods
		uint8_t GetReceivedID(void);
		uint8_t *GetUUID(void);
};

#endif /* RESPONSEID_H_ */
