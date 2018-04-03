#include <string.h>
#include "PingPong.h"

using namespace cronenberg;
using namespace std;

PingPong *PingPong::Receive(uint8_t *data, uint16_t length){
	if(length != PingPong::PING_PONG_SIZE)
		return NULL;
	
	return new PingPong((PingPong::Type)data[0]);
}

PingPong::~PingPong(void){
	// no memory location no need for destructor
}
uint16_t PingPong::Length(void) {
	return PingPong::PING_PONG_SIZE;
}

void PingPong::Parse(uint8_t *data, uint16_t *resultLength) {
	data[0] = m_type;

	*resultLength = PingPong::PING_PONG_SIZE;
}

PacketType PingPong::GetType(void) const{
	return PacketType::PingPong;
}

PingPong::Type PingPong::GetType(void){
	return (PingPong::Type)m_type;
}