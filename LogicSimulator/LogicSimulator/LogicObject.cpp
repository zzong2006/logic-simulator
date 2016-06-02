#include "stdafx.h"
#include "LogicObject.h"
#include "name_repo.h"

#define UNIT 10

void LogicObject::showSelected(Gdiplus::Graphics * gp)
{
	Gdiplus::Pen *p;
	p = new Gdiplus::Pen(Gdiplus::Color(0, 0, 0), 2);

	gp->DrawRectangle(p, top.x - 2, top.y - 2, 4, 4);
	gp->DrawRectangle(p, bottom.x - 2, top.y - 2, 4, 4);
	gp->DrawRectangle(p, top.x - 2, bottom.y - 2, 4, 4);
	gp->DrawRectangle(p, bottom.x - 2, bottom.y - 2, 4, 4);

	delete p;
}

void LogicObject::setLabel(CString input)
{
	label = input;
}

void LogicObject::set_outputCoord(int x, int y)
{
	outputCoord.x = x;
	outputCoord.y = y;
}

void LogicObject::set_inputCoord(int x, int y)
{
	switch (objectType) {
	case GATE_TYPE:
		if (objectName == NOT_GATE)
		{
			inputCoord[0].x = x - 3 * UNIT;
			inputCoord[0].y = y;
		}else {
			inputCoord[0].x = x - 6 * UNIT;
			inputCoord[0].y = y - 1 * UNIT;
			inputCoord[1].x = x - 6 * UNIT;
			inputCoord[1].y = y + 1 * UNIT;
		}
		break;

	}
	
}

void LogicObject::toggleOutput()
{
	output = !output;
}

POINT LogicObject::get_top() const
{
	return top;
}

POINT LogicObject::get_bottm() const
{
	return bottom;
}


int LogicObject::isInputSet() const
{
	int sum = 0;

	for (int i = 0; i < inputNum; i++)
	{
		if (input_line[i]->chk)
			sum++;
	}
	if (sum == inputNum)
		return 1;

	return 0;
}

LogicObject::LogicObject()
{
	output = 0;
	facing = _T("East");
}


LogicObject::~LogicObject()
{
}
