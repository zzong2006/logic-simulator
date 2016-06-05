#include "stdafx.h"
#include "FlipFlop.h"
#include "name_repo.h"

void FlipFlop::SetTrigger(CString input)
{
	if (input == _T("Rising Edge"))
		UpTrigger = TRUE;
	else UpTrigger = FALSE;
}

int FlipFlop::CheckTrigger()
{
	//inputCoord[1] 은 Clock 자리
	if (UpTrigger) {
		if (CurrClock == FALSE && inputCoord[1].second == TRUE) {
			return 1;
		}
	}
	else {
		if (CurrClock == TRUE && inputCoord[1].second == FALSE) {
			return 1;
		}
	}

	return 0;
}

void FlipFlop::setOutput()
{
	if (CheckTrigger()) {
		switch(objectName)
		{
		case D_FF:
			outputCoord[0].second = inputCoord[0].second;
			outputCoord[1].second = !(inputCoord[0].second);
			break;
		case T_FF:
			if (inputCoord[0].second) {		//T = 1
				outputCoord[0].second = !outputCoord[0].second;
				outputCoord[1].second = !outputCoord[1].second;
			}
			break;
		case JK_FF:
			break;
		}
	}
	else {		//상승,하강 트리거 적용안되면 이전 값 그래도 가져다씀.
	
	}

	CurrClock = inputCoord[1].second;
}

void FlipFlop::set_Coord_From_outC(int x, int y)
{
	top.x = x - 6 * UNIT; top.y = y - 3 * UNIT;
	bottom.x = x; bottom.y = y + 3 * UNIT;
}

FlipFlop::FlipFlop() : LogicObject()
{
	objectType = FLIPFLOP_TYPE;
	UpTrigger = TRUE;
	CurrClock = FALSE;

	outputNum = 2;			//Q , Q'

	if (objectName == JK_FF)
		inputNum = 3;
	else
		inputNum = 2;

	outputCoord[0].second = 0;
	//Q는 Q'의 반대
	outputCoord[1].second = !(outputCoord[0].second);
}


FlipFlop::~FlipFlop()
{
}
