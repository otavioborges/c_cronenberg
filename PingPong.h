#ifndef PINGPONG_H_
#define PINGPONG_H_

#include <stdint.h>
#include <stdbool.h>
#include "cronenberg.h"
#include "PacketType.h"
#include "PacketPayload.h"

class cronenberg::PingPong : public cronenberg::PacketPayload {
	private:
		static const uint16_t PING_PONG_SIZE = 1;

		uint8_t m_type;
	public:
		enum class Type{
			Ping	= 0xDA,
			Pong	= 0xAD
		};
		PingPong(PingPong::Type type):
			m_type((uint8_t)type) {};
		static PingPong* Receive(uint8_t *data, uint16_t length);
		~PingPong(void);
		uint16_t Length(void);
		void Parse(uint8_t *data, uint16_t *resultLength);
		cronenberg::PacketType GetType(void) const;

		// Get Methods
		PingPong::Type GetType(void);
};

#endif /* PINGPONG_H_ */
