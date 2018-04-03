#include <string.h>
#include "ResponseID.h"

using namespace cronenberg;
using namespace std;

ResponseID *ResponseID::Receive(uint8_t *data, uint16_t length){
	if(length != (ResponseID::DEFAULT_UUID_SIZE + 1))
		return NULL;
	
	return new ResponseID(data, data[25]);
}

ResponseID::ResponseID(uint8_t *uuid, uint8_t senderID) {
	memcpy(m_uuid, uuid, ResponseID::DEFAULT_UUID_SIZE);
	m_recvSenderID = senderID;
}

ResponseID::~ResponseID(void){
	// no memory location no need for destructor
}
uint16_t ResponseID::Length(void) {
	return (ResponseID::DEFAULT_UUID_SIZE + 1);
}

void ResponseID::Parse(uint8_t *data, uint16_t *resultLength) {
	memcpy(data, m_uuid, ResponseID::DEFAULT_UUID_SIZE);
	data[25] = m_recvSenderID;

	*resultLength = (ResponseID::DEFAULT_UUID_SIZE + 1);
}

PacketType ResponseID::GetType(void) const{
	return PacketType::ResponseID;
}

uint8_t ResponseID::GetReceivedID(void){
	return m_recvSenderID;
}

uint8_t *ResponseID::GetUUID(void){
	return m_uuid;
}
