#pragma once

#include "LogicObject.h"
#include "LineObject.h"

class Clock : public LogicObject
{

protected:
	int state;
	int cycle;			//�ֱ� 1000�� 1����. (�ʱⰪ 1000)
	int oriCycle;		//������ �ֱ� ����Ŭ (cycle�� ��� ���鼭 üũ)
public:
	void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);
	void draw_main(Gdiplus::Graphics* gp);
	void set_Coord_From_outC(int x, int y);
	void turn();
	void toggleOutput();
	void moveCycle();	//�ùķ����� ������ ����Ŭ�� ������.
	bool calOutput();	//��� ����
	Clock();
	~Clock();
};
