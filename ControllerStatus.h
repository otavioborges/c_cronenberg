#ifndef CONTROLLERSTATUS_H_
#define CONTROLLERSTATUS_H_

#include "cronenberg.h"

enum class cronenberg::ControllerStatus{
    NotInitiated        = 0x00,
    WaitingSenderID     = 0x01,
    WaitingSync         = 0x02,
    Running             = 0x03
};

#endif /* CONTROLLERSTATUS_H_ */