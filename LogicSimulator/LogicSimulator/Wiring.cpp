#include "stdafx.h"
#include "Wiring.h"
#include "name_repo.h"

Wiring::Wiring() : LogicObject()
{
	objectType = WIRING_TYPE;
	if(objectName)
	inputNum = 1;
	outputNum = 1;
}


Wiring::~Wiring()
{
}
