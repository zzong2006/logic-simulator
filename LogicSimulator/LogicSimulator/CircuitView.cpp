// CircuitView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LogicSimulator.h"
#include "CircuitView.h"
#include "LogicSimulatorDoc.h"

#include <gdiplus.h>
// CCircuitView

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
	ON_COMMAND(ID_CLICK_MODE, &CCircuitView::OnClickMode)
	ON_COMMAND(ID_SELECT_MODE, &CCircuitView::OnSelectMode)
	ON_UPDATE_COMMAND_UI(ID_CLICK_MODE, &CCircuitView::OnUpdateClickMode)
	ON_UPDATE_COMMAND_UI(ID_SELECT_MODE, &CCircuitView::OnUpdateSelectMode)
END_MESSAGE_MAP()


// CCircuitView �׸����Դϴ�.

void CCircuitView::OnDraw(CDC* pDC)
{

}


void CCircuitView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();
	Gdiplus::Graphics graphics(dc);
	Gdiplus::Pen P(Gdiplus::Color(0, 0, 0), 2);

	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
	CRect rect;
	GetClientRect(&rect);

	for (int i = 0; i < rect.right; i += UNIT)
	{
		for (int j = 0; j < rect.bottom; j += UNIT)
		{
			dc.SetPixel(i, j, RGB(128, 128, 128));
		}
	}

	if (!pDoc->isSelected) {
		for (int i = 0; i < pDoc->logicInfo.size(); i++)
		{
			pDoc->logicInfo.at(i)->draw_main(&graphics);
		}
	}
}


// CCircuitView �����Դϴ�.

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


// CCircuitView �޽��� ó�����Դϴ�.


void CCircuitView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	CClientDC dc(this);
	Gdiplus::Graphics graphics(dc);
	Gdiplus::Pen P(Gdiplus::Color(0, 0, 0),2);
	
	int dec_x, dec_y;
	dec_x = point.x - point.x % UNIT;
	dec_y = point.y - point.y % UNIT;
	//and east
	

	if (pDoc->isSelected) {
		LogicObject *temp;
		if (pDoc->selectedType == _T("AND Gate")) {
			temp = new andGate();
		}
		else if(pDoc->selectedType == _T("OR Gate")) {
			temp = new orGate();
		}
		else if (pDoc->selectedType == _T("Pin")) {
			temp = new Pin();
		}

		if (temp != NULL) {
			temp->set_outputCoord(dec_x, dec_y);
			temp->set_Coord_From_outC(dec_x, dec_y);
			pDoc->logicInfo.push_back(temp);	
		}

		free(pDoc->temp);
		pDoc->temp = NULL;

		pDoc->isSelected = false;
	}

	if (pDoc->clickMode) {
		CPoint pos;
		
		GetCursorPos(&pos);
		ScreenToClient(&pos);

		for (int i = 0; i < pDoc->logicInfo.size(); i++) {
			POINT temp_top, temp_bottom;
			temp_top = pDoc->logicInfo.at(i)->get_top();
			temp_bottom = pDoc->logicInfo.at(i)->get_bottm();

			CRect rect(temp_top.x, temp_top.y, temp_bottom.x, temp_bottom.y);

			if (PtInRect(rect,pos)) {
				pDoc->logicInfo.at(i)->toggleOutput();
			}
		}
	}
	Invalidate();

	CView::OnLButtonDown(nFlags, point);
}


BOOL CCircuitView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *) GetDocument();
	
	if (nHitTest==HTCLIENT)
	{
		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient(&point);

		if (pDoc->isSelected)
			::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR1));
		else if(pDoc->selectMode)
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		else if(pDoc->clickMode)
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

	Gdiplus::Pen P(Gdiplus::Color(190,190,190), 2);
	Gdiplus::Pen DP(Gdiplus::Color(255, 255, 255), 2);

	dec_x = point.x - point.x % UNIT;
	dec_y = point.y - point.y % UNIT;
	
	//and east
	if (pDoc->isSelected) {
		if (pDoc->selectedType == _T("AND Gate")) {
			if(pDoc->temp == NULL)
				pDoc->temp = new andGate();
		}
		else if (pDoc->selectedType == _T("OR Gate")) {
			if (pDoc->temp == NULL)
				pDoc->temp = new orGate();
		}
		else if (pDoc->selectedType == _T("Pin")) {
			if (pDoc->temp == NULL)
				pDoc->temp = new Pin();
		}
		//�ٸ� �޴��� ���������� ������ ����ȴٸ� �� ������ if�� �������� ������ �ذ��.

		if (pDoc->temp != NULL) {
			pDoc->temp->draw_shadow(&graphics, &DP);
			pDoc->temp->set_outputCoord(dec_x, dec_y);
			pDoc->temp->draw_shadow(&graphics, &P);

		}
	}
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	

	CView::OnMouseMove(nFlags, point);
}


void CCircuitView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();
	pDoc->isSelected = false;

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}



void CCircuitView::OnClickMode()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pDoc->clickMode = TRUE;
	pDoc->selectMode = FALSE;

}


void CCircuitView::OnSelectMode()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pDoc->clickMode = FALSE;
	pDoc->selectMode = TRUE;
}


void CCircuitView::OnUpdateClickMode(CCmdUI *pCmdUI)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pCmdUI->SetCheck(pDoc->clickMode == TRUE);
}


void CCircuitView::OnUpdateSelectMode(CCmdUI *pCmdUI)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();
	
	pCmdUI->SetCheck(pDoc->selectMode == TRUE);
}