#pragma once
#include "Gate.h"

class norGate : public Gate
{
public:
	void draw_main(Gdiplus::Graphics* gp);
	void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);
	void set_Coord_From_outC(int x, int y);
	void set_output();
	virtual void turn();

	norGate();
	norGate(int dec_x, int dec_y);
	virtual ~norGate();
};

