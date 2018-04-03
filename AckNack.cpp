#include <string.h>
#include "AckNack.h"

using namespace cronenberg;
using namespace std;

AckNack *AckNack::Receive(uint8_t *data, uint16_t length){
	if(length != AckNack::ACK_NACK_SIZE)
		return NULL;
	
	return new AckNack((AckNack::Result)data[0], data[1], data[2]);
}

AckNack::~AckNack(void){
	// no memory location no need for destructor
}
uint16_t AckNack::Length(void) {
	return AckNack::ACK_NACK_SIZE;
}

void AckNack::Parse(uint8_t *data, uint16_t *resultLength) {
	data[0] = m_ack;
	data[1] = m_sender;
	data[2] = m_packetID;

	*resultLength = AckNack::ACK_NACK_SIZE;
}

PacketType AckNack::GetType(void) const{
	return PacketType::AckNack;
}

AckNack::Result AckNack::GetResult(void){
	return (AckNack::Result)m_ack;
}

uint8_t AckNack::GetSender(void){
	return m_sender;
}

uint8_t AckNack::GetPacketID(void){
	return m_packetID;
}
