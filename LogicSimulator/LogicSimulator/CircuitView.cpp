// CircuitView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LogicSimulator.h"
#include "CircuitView.h"
#include "LogicSimulatorDoc.h"
#include "LineObject.h"

#include <gdiplus.h>
#include <vector>

using std::vector;

// CCircuitView

int object = OBJECT;
int cur_line;
CPoint line[3];
int move_state = 0;
vector < LineObject *> lines;

IMPLEMENT_DYNCREATE(CCircuitView, CView)

CCircuitView::CCircuitView()
{

}

CCircuitView::~CCircuitView()
{

}

BEGIN_MESSAGE_MAP(CCircuitView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_CLICK_MODE, &CCircuitView::OnClickMode)
	ON_COMMAND(ID_SELECT_MODE, &CCircuitView::OnSelectMode)
	ON_UPDATE_COMMAND_UI(ID_CLICK_MODE, &CCircuitView::OnUpdateClickMode)
	ON_UPDATE_COMMAND_UI(ID_SELECT_MODE, &CCircuitView::OnUpdateSelectMode)
	ON_COMMAND(ID_ON_SIMULATE, &CCircuitView::OnOnSimulate)
	ON_UPDATE_COMMAND_UI(ID_ON_SIMULATE, &CCircuitView::OnUpdateOnSimulate)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCircuitView 그리기입니다.

void CCircuitView::OnDraw(CDC* pDC)
{

}


void CCircuitView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();
	Gdiplus::Graphics graphics(dc);
	Gdiplus::Pen P(Gdiplus::Color(0, 0, 0), 2);

	// TODO: 여기에 그리기 코드를 추가합니다.
	CRect rect;
	GetClientRect(&rect);

	CheckCircuit();

	if (!pDoc->isSelected) {
		for (int i = 0; i < pDoc->logicInfo.size(); i++)
		{
			pDoc->logicInfo.at(i)->draw_main(&graphics);
		}
	}

	LineObject* templine;

	for (int i = 0; i < lines.size(); i++)
		lines.at(i)->draw_main(&graphics);

}


// CCircuitView 진단입니다.

#ifdef _DEBUG
void CCircuitView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CCircuitView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCircuitView 메시지 처리기입니다.

/*
	마우스 왼쪽 클릭했을때 일어나야 할 일들
	1. 메뉴에서 논리 오브젝트 선택하고 오른쪽에서 선택하면
	그려진다.
	2. 메뉴에서 논리 오브젝트 선택하지 않고 (isSelected 가 False 인 경우) field 에서 선택하면 표시가 나면서 선택되어짐.
	3. 

*/
void CCircuitView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	CClientDC dc(this);
	Gdiplus::Graphics graphics(dc);
	Gdiplus::Pen P(Gdiplus::Color(0, 0, 0), 2);

	int dec_x, dec_y;
	dec_x = point.x - point.x % UNIT;
	dec_y = point.y - point.y % UNIT;

	//선을 선택했을 경우는 LINE , 기본값은 OBJECT로 함.
	object = OBJECT;

	// 선들중 하나가 마우스 포인터 위에 있다고 하면
	// 그 선을 선택하고 움직이게함.
	for (int i = 0; i < lines.size(); i++) {
		if (lines.at(i)->IS_match_mouseCoord(point)) {
			object = LINE;
			break;
		}
	}

	if (object == OBJECT)
	{
		//메뉴에서 선택하고 필드에서 지정 완료될 때
		LogicObject* temp;
		temp = NULL;
		if (pDoc->isSelected) {
			if (pDoc->objectType == GATE_TYPE)
			{
				Gate *Gtemp;
				Gtemp = NULL;

				switch (pDoc->objectName)
				{
				case AND_GATE:
					Gtemp = new andGate();
					Gtemp->objectName = AND_GATE;
					Gtemp->objectType = GATE_TYPE;
					Gtemp->input_line[0] = NULL;
					Gtemp->input_line[1] = NULL;
					Gtemp->output_line = NULL;
					break;
				case OR_GATE:
					Gtemp = new orGate();
					Gtemp->objectName = OR_GATE;
					Gtemp->objectType = GATE_TYPE;
					Gtemp->input_line[0] = NULL;
					Gtemp->input_line[1] = NULL;
					Gtemp->output_line = NULL;
					break;
				}

				if (Gtemp != NULL) {
					Gtemp->set_outputCoord(dec_x, dec_y);
					Gtemp->set_Coord_From_outC(dec_x, dec_y);
					pDoc->gateInfo.push_back(Gtemp);
					pDoc->logicInfo.push_back(Gtemp);
				}
			}
			else if (pDoc->objectType == WIRING_TYPE)
			{
				Pin *Ptemp;
				Clock *Ctemp;
				temp = NULL;

				switch (pDoc->objectName)
				{
				case PIN :
					Ptemp = new Pin();
					Ptemp->objectType = WIRING_TYPE;
					Ptemp->objectName = PIN;
					Ptemp->connect_line = NULL;
					pDoc->pinInfo.push_back(Ptemp);
					temp = Ptemp;
					break;

				case CLOCK:
					Ctemp = new Clock();
					Ctemp->objectType = WIRING_TYPE;
					Ctemp->objectName = CLOCK;
					Ctemp->connect_line = NULL;
					pDoc->clockInfo.push_back(Ctemp);
					temp = Ctemp;
					break;
				}
				if (temp != NULL) {
					temp->set_outputCoord(dec_x, dec_y);
					temp->set_Coord_From_outC(dec_x, dec_y);
					pDoc->logicInfo.push_back(temp);
				}
			}

			delete pDoc->temp;
			pDoc->temp = NULL;
			pDoc->isSelected = false;
		}

		//클릭 모드인 경우 
		//Pin 과 Clock 의 output 데이터를 바꿀뿐 gate 는 영향이 없다..
		if (pDoc->clickMode) {
			CPoint pos;

			GetCursorPos(&pos);
			ScreenToClient(&pos);
			POINT temp_top, temp_bottom;

			for (int i = 0; i < pDoc->pinInfo.size(); i++)
			{
				
				temp_top = pDoc->pinInfo.at(i)->get_top();
				temp_bottom = pDoc->pinInfo.at(i)->get_bottm();

				CRect rect(temp_top.x, temp_top.y, temp_bottom.x, temp_bottom.y);

				if (PtInRect(rect, pos)) 
					pDoc->pinInfo.at(i)->toggleOutput();
			}

			for (int i = 0; i < pDoc->clockInfo.size(); i++)
			{
				temp_top = pDoc->clockInfo.at(i)->get_top();
				temp_bottom = pDoc->clockInfo.at(i)->get_bottm();

				CRect rect(temp_top.x, temp_top.y, temp_bottom.x, temp_bottom.y);

				if (PtInRect(rect, pos))
					pDoc->clockInfo.at(i)->toggleOutput();
			}
		}
	} else {		//위에서 LINE MODE인것을 확인하고 시작함
		LineObject* temp_line = new LineObject();
		cur_line = lines.size();
		lines.push_back(temp_line);
		temp_line->state = OFF_SIGNAL;
		
		for (int i = 0; i < pDoc->gateInfo.size(); i++)
		{
			int index = 0;
			if (pDoc->gateInfo.at(i)->Is_match_outputCoord(point))
			{
				pDoc->gateInfo.at(i)->output_line = temp_line;
			}
			else if ((index = pDoc->gateInfo.at(i)->Is_match_inputCoord(point)) != -1)
			{
				pDoc->gateInfo.at(i)->input_line[index] = temp_line;
			}
		}

		for (int i = 0; i < pDoc->pinInfo.size(); i++)
		{
			if (pDoc->pinInfo.at(i)->Is_match_outputCoord(point))
			{
				pDoc->pinInfo.at(i)->connect_line = temp_line;
			}
		}

		for (int i = 0; i < lines.size(); i++)
		{
			if (i != cur_line && lines.at(i)->Is_match_IineCoord(point))
				lines.at(i)->connect_lines.push_back(temp_line);
		}

		temp_line->line[0] = CPoint(dec_x, dec_y);
		temp_line->line[1] = CPoint(dec_x, dec_y);
		temp_line->line[0] = CPoint(dec_x, dec_y);
	}

	Invalidate();

	CView::OnLButtonDown(nFlags, point);
}

//선을 돌아가면서 0,1 값을 체크한다.
void CCircuitView::CheckCircuit()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	for (int i = 0; i < pDoc->pinInfo.size(); i++)
	{
		if (pDoc->pinInfo.at(i)->connect_line != NULL)
		{
			pDoc->pinInfo.at(i)->connect_line->state = (pDoc->pinInfo.at(i)->output == TRUE) ? ON_SIGNAL : OFF_SIGNAL;
			pDoc->pinInfo.at(i)->connect_line->check_connect();
		}
	}

	for (int i = 0; i < pDoc->gateInfo.size(); i++)
	{
		if (pDoc->gateInfo.at(i)->output_line != NULL && pDoc->gateInfo.at(i)->input_line[0] != NULL && pDoc->gateInfo.at(i)->input_line[1] != NULL
			&& pDoc->gateInfo.at(i)->input_line[0]->state == ON_SIGNAL && pDoc->gateInfo.at(i)->input_line[1]->state == ON_SIGNAL)
		{
			pDoc->gateInfo.at(i)->output_line->state = ON_SIGNAL;
		}
		else if (pDoc->gateInfo.at(i)->output_line != NULL)
			pDoc->gateInfo.at(i)->output_line->state = OUTPUT_SIGNAL;
	}
}


BOOL CCircuitView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	if (nHitTest == HTCLIENT)
	{
		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient(&point);

		//배치 모드일땐 십자 표시
		if (pDoc->isSelected)
			::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR1));
		//선택 모드일땐 화살표 표시
		else if (pDoc->selectMode)
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		//클릭 모드일땐 손모양 표시
		else if (pDoc->clickMode)
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));

		return TRUE;
	}

	return CView::OnSetCursor(pWnd, nHitTest, message);
}


void CCircuitView::OnMouseMove(UINT nFlags, CPoint point)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	CClientDC dc(this);
	Gdiplus::Graphics graphics(dc);

	Gdiplus::Pen P(Gdiplus::Color(190, 190, 190), 2);
	Gdiplus::Pen DP(Gdiplus::Color(255, 255, 255), 2);

	dec_x = point.x - point.x % UNIT;
	dec_y = point.y - point.y % UNIT;

	//선택 된 상태를 보여주려 하는 코드 같은데,
	//이 부분은 문제가 있다. 왜냐면 모든 line 을 담은 곳을 보면서 확인하면 한번에 해결되기 때문이다.


	//메뉴에서 오브젝트가 선택된 상태라면 움직이면
	//오브젝트가 그려지게 된다.

	if (object == OBJECT)
	{
		if (pDoc->isSelected) {
			if (pDoc->temp == NULL) {
				if (pDoc->objectType == GATE_TYPE)
				{
					switch (pDoc->objectName)
					{
					case AND_GATE:
						pDoc->temp = new andGate();
						break;
					case OR_GATE:
						pDoc->temp = new orGate();
						break;
					}
				}
				else if (pDoc->objectType == WIRING_TYPE)
				{
					switch (pDoc->objectName)
					{
					case PIN:
						pDoc->temp = new Pin();
						break;
					case CLOCK:
						pDoc->temp = new Clock();
						break;
					}
				}
			}
			
			if (pDoc->temp != NULL) {
				pDoc->temp->draw_shadow(&graphics, &DP);
				pDoc->temp->set_outputCoord(dec_x, dec_y);
				pDoc->temp->draw_shadow(&graphics, &P);
			}
		}

	} else {
		// LINE mode 에서 왼쪽 버튼 누르며 mouse가 움직일때
		if (nFlags == MK_LBUTTON)
		{
			LineObject* temp_line = lines.at(cur_line);
			if (move_state == HTOV)
			{
				temp_line->line[1].y = temp_line->line[0].y;
				temp_line->line[1].x = dec_x;
				temp_line->line[2].x = temp_line->line[1].x;
				temp_line->line[2].y = dec_y;

				if (dec_x == temp_line->line[0].x)
					move_state = VTOH;
			}
			else if (move_state == VTOH)
			{
				temp_line->line[1].x = temp_line->line[0].x;
				temp_line->line[1].y = dec_y;
				temp_line->line[2].y = temp_line->line[1].y;
				temp_line->line[2].x = dec_x;

				if (dec_y == temp_line->line[0].y)
					move_state = HTOV;
			}
			else{
				if (dec_y != temp_line->line[0].y)
					move_state = VTOH;
				else if (dec_x != temp_line->line[0].x)
					move_state = HTOV;
			}
			Invalidate();
		}
	}
	

	CView::OnMouseMove(nFlags, point);
}


void CCircuitView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();
	pDoc->isSelected = false;

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}



BOOL CCircuitView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return FALSE;
}


void CCircuitView::OnMouseLeave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CView::OnMouseLeave();
}


void CCircuitView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	//그리기를 마쳤을때 하는 behavior
	if (object == LINE)
	{
		for (int i = 0; i < lines.size(); i++)
		{
			if (i != cur_line && lines.at(i)->Is_match_IineCoord(point))
			{
				lines.at(i)->connect_lines.push_back(lines.at(cur_line));
				lines.at(cur_line)->connect_lines.push_back(lines.at(i));
			}
		}

		for (int i = 0; i < pDoc->gateInfo.size(); i++)
		{
			int index = 0;
			if (pDoc->gateInfo.at(i)->Is_match_outputCoord(point))
			{
				pDoc->gateInfo.at(i)->output_line = lines.at(cur_line);
			}
			else if ((index = pDoc->gateInfo.at(i)->Is_match_inputCoord(point)) != -1)
			{
				pDoc->gateInfo.at(i)->input_line[index] = lines.at(cur_line);
			}
		}

		for (int i = 0; i < pDoc->pinInfo.size(); i++)
		{
			if (pDoc->pinInfo.at(i)->Is_match_outputCoord(point))
			{
				pDoc->pinInfo.at(i)->connect_line = lines.at(cur_line);
			}
		}
		object = OBJECT;
	}

	Invalidate();
	CView::OnLButtonUp(nFlags, point);
}

//클릭 모드 버튼을 클릭했을 경우
void CCircuitView::OnClickMode()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pDoc->clickMode = TRUE;
	pDoc->selectMode = FALSE;

}

//선택 모드 버튼을 클릭했을 경우
void CCircuitView::OnSelectMode()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pDoc->clickMode = FALSE;
	pDoc->selectMode = TRUE;
}

//클릭 모드를 체크할때의 조건 확인
void CCircuitView::OnUpdateClickMode(CCmdUI *pCmdUI)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pCmdUI->SetCheck(pDoc->clickMode == TRUE);
}

//선택모드를 체크할 때의 조건 확인
void CCircuitView::OnUpdateSelectMode(CCmdUI *pCmdUI)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pCmdUI->SetCheck(pDoc->selectMode == TRUE);
}


void CCircuitView::OnOnSimulate()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	if (pDoc->simulateMode) {
		pDoc->simulateMode = FALSE;
		KillTimer(0);
	}
	else {
		pDoc->simulateMode = TRUE;
		SetTimer(0, 1000, NULL);
	}
		
}


void CCircuitView::OnUpdateOnSimulate(CCmdUI *pCmdUI)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pCmdUI->SetCheck(pDoc->simulateMode == TRUE);
}


/*시뮬레이션 모드로 들어갈때 돌아갈 이벤트
*모든 Clock의 cycle을 1000씩 빼준다 (1초마다) 
*만약 cycle이 0이 될 경우 output toggle 및 cycle 초기화
*/
void CCircuitView::OnTimer(UINT_PTR nIDEvent)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	for (int i = 0; i < pDoc->clockInfo.size(); i++) {
		pDoc->clockInfo.at(i)->moveCycle();
	}

	Invalidate();
	CView::OnTimer(nIDEvent);
}
