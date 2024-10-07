#include "Engine.h"
#include "Includes.h"



#define JumpAddress 0x38399B5C
#define FrunAddress 0x38399AF0
#define SrunAddress 0x38399B14
#define BrunAddress 0x38399B38


void PlayerMovements() {

	if (Features::Playermovement::SpeedHack == 1) {
		*(float*)FrunAddress = 1000.0f;
		*(float*)SrunAddress = 1000.0f;
		*(float*)BrunAddress = 1000.0f;

	}
	else{
		*(float*)FrunAddress = 285.0f;
		*(float*)SrunAddress = 285.0f;
		*(float*)BrunAddress = 285.0f;

	}


}