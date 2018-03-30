#ifndef DATATYPE_H_
#define DATATYPE_H_

#include "cronenberg.h"

enum class cronenberg::DataType {
	Booleans = 0x00,
	UsignedShort = 0x01,
	SignedShort = 0x02,
	UnsginedInteger = 0x03,
	SignedInteger = 0x04,
	UnsignedLong = 0x05,
	SignedLong = 0x06,
	Float = 0x07
};

#endif /* DATATYPE_H_ */
