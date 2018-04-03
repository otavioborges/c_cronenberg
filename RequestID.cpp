#include <string.h>
#include "RequestID.h"

using namespace cronenberg;
using namespace std;

RequestID *RequestID::Receive(uint8_t *data, uint16_t length){
	if(length != RequestID::DEFAULT_UUID_SIZE)
		return NULL;
	
	return new RequestID(data);
}

RequestID::~RequestID(void){
	// no memory location no need for destructor
}
uint16_t RequestID::Length(void) {
	return RequestID::DEFAULT_UUID_SIZE;
}

void RequestID::Parse(uint8_t *data, uint16_t *resultLength) {
	memcpy(data, m_uuid, RequestID::DEFAULT_UUID_SIZE);

	*resultLength = RequestID::DEFAULT_UUID_SIZE;
}

PacketType RequestID::GetType(void) const{
	return PacketType::RequestID;
}

uint8_t *RequestID::GetUUID(void){
	return m_uuid;
}
