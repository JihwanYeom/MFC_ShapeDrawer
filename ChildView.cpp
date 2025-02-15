
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "Assignment4.h"
#include "ChildView.h"
#include "CMyShape.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	mode = RECT; // 시작 시 모드는 사각형 모드로 시작
	radius = 0; // radius 초기화
	dragSelect = false; // 선택 드래그 모드 false로 초기화
	color = RGB(rand() % 127 + 128, rand() % 127 + 128, rand() % 127 + 128); // 랜덤 색상 저장
	// star 배열 초기화
	for (int i = 0; i < 10; i++)
		star[i] = 0;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_DRAW_RECT, &CChildView::OnDrawRect)
	ON_COMMAND(ID_DRAW_CIRCLE, &CChildView::OnDrawCircle)
	ON_COMMAND(ID_DRAW_CURVE, &CChildView::OnDrawCurve)
	ON_COMMAND(ID_DRAW_STAR, &CChildView::OnDrawStar)
	ON_COMMAND(ID_SELECT, &CChildView::OnSelect)
	ON_COMMAND(ID_GROUP, &CChildView::OnGroup)
	ON_COMMAND(ID_UNGROUP, &CChildView::OnUngroup)
	ON_COMMAND(ID_ALIGN_FRONT, &CChildView::OnAlignFront)
	ON_COMMAND(ID_ALIGN_BACK, &CChildView::OnAlignBack)
	ON_UPDATE_COMMAND_UI(ID_DRAW_RECT, &CChildView::OnUpdateDrawRect)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CIRCLE, &CChildView::OnUpdateDrawCircle)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CURVE, &CChildView::OnUpdateDrawCurve)
	ON_UPDATE_COMMAND_UI(ID_DRAW_STAR, &CChildView::OnUpdateDrawStar)
	ON_UPDATE_COMMAND_UI(ID_SELECT, &CChildView::OnUpdateSelect)
	ON_UPDATE_COMMAND_UI(ID_GROUP, &CChildView::OnUpdateGroup)
	ON_UPDATE_COMMAND_UI(ID_UNGROUP, &CChildView::OnUpdateUngroup)
	ON_WM_CONTEXTMENU()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this);
	
	/*------------------------더블 버퍼링------------------------*/

	CRect rect;
	GetClientRect(rect);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	CDC memDC;

	memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(bmp);
	memDC.SelectStockObject(WHITE_BRUSH);
	memDC.SelectStockObject(WHITE_PEN);
	memDC.Rectangle(rect);

	/*---------------------------그리기--------------------------*/
	
	// Shapes를 순회하며 도형을 그리고 선택되어있는 도형의 경우 빨간 점선으로 된 사각형을 그림
	POSITION POS1 = Shapes.GetHeadPosition();
	while (POS1 != NULL)
	{
		POSITION CUR_POS = POS1;
		Shapes.GetNext(POS1)->draw(memDC);
		POSITION POS2 = Selected.GetHeadPosition();
		while (POS2 != NULL)
		{	
			if (CUR_POS == Selected.GetNext(POS2))
			{
				Shapes.GetAt(CUR_POS)->drawBox(memDC);
				break;
			}
		}
	}

	// 모드에 따라 현재 그리고 있는 도형을 그려냄
	if (mode == RECT)
	{
		CBrush brush(color);
		CPen pen(PS_SOLID, 3, RGB(0, 0, 0));
		memDC.SelectObject(brush);
		memDC.SelectObject(pen);
		memDC.Rectangle(p1.x, p1.y, p2.x, p2.y);
	}
	if (mode == CIRCLE)
	{
		CBrush brush(color);
		CPen pen(PS_SOLID, 3, RGB(0, 0, 0));
		memDC.SelectObject(brush);
		memDC.SelectObject(pen);
		memDC.Ellipse(p1.x - radius, p1.y - radius, p1.x + radius, p1.y + radius);
	}
	if (mode == CURVE)
	{
		CPen pen(PS_SOLID, 3, color);
		memDC.SelectObject(pen);
		POSITION POS1 = pts.GetHeadPosition();
		POSITION POS2 = pts.GetHeadPosition();
		if (POS1 != NULL)
			pts.GetNext(POS1);
		while (POS1 != NULL)
		{
			memDC.MoveTo(pts.GetNext(POS1));
			memDC.LineTo(pts.GetNext(POS2));
		}
	}
	if (mode == STAR)
	{
		// 10개의 점을 이용해 rgn으로 별의 형태를 만듦
		CBrush brush(color);
		CBrush black(RGB(0, 0, 0));
		CPen pen(PS_SOLID, 3, RGB(0, 0, 0));
		memDC.SelectObject(brush);
		memDC.SelectObject(pen);
		CRgn rgn;
		rgn.CreatePolygonRgn(star, 10, ALTERNATE);
		memDC.PaintRgn(&rgn);
		memDC.FrameRgn(&rgn, &black, 3, 3);
	}
	// 드래그 선택 상태일 경우 검은색 점선으로 범위를 표시함
	if (mode == SELECT && dragSelect)
	{
		CPen pen(PS_DOT, 1, RGB(0, 0, 0));
		memDC.SelectStockObject(NULL_BRUSH);
		memDC.SelectObject(pen);
		memDC.Rectangle(p1.x, p1.y, p2.x, p2.y);
	}
	
	// 도형의 개수와 선택된 도형의 개수를 출력하는 부분
	CString str1;
	str1.Format(_T("도형의 개수 : %d"), Shapes.GetSize());
	memDC.TextOutW(0, 0, str1);

	CString str2;
	str2.Format(_T("선택된 도형의 개수 : %d"), Selected.GetSize());
	memDC.TextOutW(0, 20, str2);

	// BitBlt로 memDC의 화면을 dc로 전송
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 모드에 따라 마우스 클릭 시 동작 구분
	if (mode == RECT)
	{
		SetCapture();
		p1 = point;
		p2 = point;
	}
	if (mode == CIRCLE)
	{
		SetCapture();
		p1 = point;
		p2 = point;
	}
	if (mode == CURVE)
	{
		SetCapture();
		pts.AddTail(point);
	}
	if (mode == STAR)
	{
		SetCapture();
		p1 = point;
		p2 = point;
	}
	// 선택 모드 시 동작
	if (mode == SELECT)
	{
		SetCapture();
		p1 = point;
		p2 = point;
		// 도형을 클릭하지 않았을 경우, 드래그 선택모드로 진입
		dragSelect = true;
		// Shift가 눌리지 않았을 시 중복 선택 하지 않음
		if (!(nFlags & MK_SHIFT))
			Selected.RemoveAll();
		POSITION POS = Shapes.GetTailPosition();
		POSITION CUR_POS;
		// 위에 있는 도형을 우선순위로, 선택 리스트에 중복된 도형이 없도록 삽입
		while (POS != NULL)
		{
			CUR_POS = POS;
			if (Shapes.GetPrev(POS)->Is_in(p1) == true)
			{
				POSITION POS2 = Selected.GetHeadPosition();
				bool not_exist = true;
				while(POS2 != NULL)
				{
					if (Selected.GetNext(POS2) == CUR_POS)
					{
						not_exist = false;
						break;
					}
				}
				if (not_exist)
					Selected.AddTail(CUR_POS);
				dragSelect = false;
				break;
			}
		}
	}
	Invalidate();
	CWnd::OnLButtonDown(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// 모드에 따라 좌클릭 + 드래그 시 동작 구분
	if (nFlags & MK_LBUTTON)
	{
		if (mode == RECT)
		{
			p2 = point;
		}
		if (mode == CIRCLE)
		{
			p2 = point;
			// 원의 경우 그리는 과정을 보이기 위해 반지름을 계산
			radius = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
		}
		if (mode == CURVE)
		{
			// 선의 경우 점을 계속해서 CPoint 배열에 저장
			pts.AddTail(point);
		}
		if (mode == STAR)
		{
			p2 = point;
			// 별의 경우도 그리는 과정을 보이기 위해 반지름을 계산
			radius = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
			// 별의 규칙성을 이용해 10개의 점을 선정하여 추후 rgn 객체의 매개변수로 사용
			for (int i = 0; i < 10; i++)
			{
				float theta = (i * 36 + 90) / (float)180 * 3.141592;
				if (i % 2 == 0)
					star[i] = CPoint(p1.x - cos(theta) * radius, p1.y - sin(theta) * radius);
				if (i % 2 == 1)
					star[i] = CPoint(p1.x - cos(theta) * radius/2.6, p1.y - sin(theta) * radius/2.6);
			}
		}
		if (mode == SELECT)
		{
			// 드래그 선택 모드인 경우 마우스만 계속 추적
			if(dragSelect)
				p2 = point;
			// 선택 및 이동상태일 경우 p1, p2를 계속 갱신하여 dx, dy를 구해 도형 이동
			else
			{
				p2 = point;
				POSITION POS = Selected.GetHeadPosition();
				int dx = p2.x - p1.x;
				int dy = p2.y - p1.y;
				while (POS != NULL)
				{
					Shapes.GetAt(Selected.GetNext(POS))->move(dx,dy);
				}
				p1 = p2;
			}
			
		}
	}
	Invalidate();
	CWnd::OnMouseMove(nFlags, point);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// 직사각형의 경우 p1과 p2의 위치관계를 다시 정리하여 CMyRect의 형태로 Shapes에 저장
	if (mode == RECT)
	{
		CPoint p, q, center;
		p = p1;
		q = p2;
		if (p.x > q.x)
		{
			q.x = p.x;
			p.x = p2.x;
		}
		if (p.y > q.y)
		{
			q.y = p.y;
			p.y = p2.y;
		}
		center.x = (p.x + q.x) / 2;
		center.y = (p.y + q.y) / 2;
		CMyRect* rect = new CMyRect(center, color, q.x - p.x, q.y - p.y);
		Shapes.AddTail(rect);
		p1 = CPoint(0, 0);
		p2 = CPoint(0, 0);
		ReleaseCapture();
		color = RGB(rand() % 127 + 128, rand() % 127 + 128, rand() % 127 + 128);
	}
	// 원의 경우 원의 중심과 과 반지름을 CMyCircle의 형태로 Shapes에 저장
	if (mode == CIRCLE)
	{
		CMyCircle* circle = new CMyCircle(p1, color, radius);
		Shapes.AddTail(circle);
		p1 = CPoint(0, 0);
		p2 = CPoint(0, 0);
		radius = 0;
		ReleaseCapture();
		color = RGB(rand() % 127 + 128, rand() % 127 + 128, rand() % 127 + 128);
	}
	// 곡선의 경우 선을 이루는 CPoint 리스트를 CMyLine의 형태로 Shapes에 저장
	if (mode == CURVE)
	{
		CMyLine* L = new CMyLine(CPoint(0,0), color, pts);
		Shapes.AddTail(L);
		pts.RemoveAll();
		ReleaseCapture();
		color = RGB(rand() % 127 + 128, rand() % 127 + 128, rand() % 127 + 128);
	}
	// 별의 경우 별을 이루는 10개의 꼭짓점 배열을 CMyStar의 형태로 Shapes에 저장
	if (mode == STAR)
	{
		CMyStar* S = new CMyStar(p1, color, star);
		Shapes.AddTail(S);
		p1 = CPoint(0, 0);
		p2 = CPoint(0, 0);
		radius = 0;
		for (int i = 0; i < 10; i++)
			star[i] = 0;
		ReleaseCapture();
		color = RGB(rand() % 127 + 128, rand() % 127 + 128, rand() % 127 + 128);
	}
	if (mode == SELECT)
	{
		// 드래그 선택 모드일 경우 현재 범위 안에 속한 도형들을 선택 리스트에 삽입
		if (dragSelect)
		{
			CPoint p, q;
			p = p1;
			q = p2;
			if (p.x > q.x)
			{
				q.x = p.x;
				p.x = p2.x;
			}
			if (p.y > q.y)
			{
				q.y = p.y;
				p.y = p2.y;
			}
			CRect SRect = CRect(p, q);
			POSITION POS = Shapes.GetHeadPosition();
			POSITION CUR_POS;
			while (POS != NULL)
			{
				CUR_POS = POS;
				if (Shapes.GetNext(POS)->Is_in_box(SRect) == true)
				{
					Selected.AddTail(CUR_POS);
				}
			}
			dragSelect = false;
		}	
		p1 = CPoint(0, 0);
		p2 = CPoint(0, 0);
		ReleaseCapture();
	}
	Invalidate();
	CWnd::OnLButtonUp(nFlags, point);
}


void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// 선택 모드 중 DELETE키가 눌렸을 경우 선택된 도형들을 삭제함
	if (mode == SELECT && nChar == VK_DELETE)
	{
		POSITION POS = Selected.GetHeadPosition();
		while (POS != NULL)
		{
			Shapes.RemoveAt(Selected.GetNext(POS));
		}
		Selected.RemoveAll();
	}
	Invalidate();
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

// 메뉴 및 툴바 선택에 따라 모드 변경, 그리기 모드 시 선택 리스트를 비움
void CChildView::OnDrawRect()
{
	Selected.RemoveAll();
	mode = RECT;
}

void CChildView::OnDrawCircle()
{
	Selected.RemoveAll();
	mode = CIRCLE;
}

void CChildView::OnDrawCurve()
{
	Selected.RemoveAll();
	mode = CURVE;
}

void CChildView::OnDrawStar()
{
	Selected.RemoveAll();
	mode = STAR;
}

void CChildView::OnSelect()
{
	mode = SELECT;
}

// 그룹화
void CChildView::OnGroup()
{
	// m_child를 가진 CMyShape포인터를 하나 만든 뒤, 선택된 개체들을 m_child에 삽입하고.
	// 선택되었던 개체들을 삭제한 뒤 m_child를 자손으로 가지는 CMyShape 객체를 Shapes에 삽입한다.
	CList <CMyShape*> temp;
	POSITION POS = Selected.GetHeadPosition();
	if (POS == NULL)
		return;
	while (POS != NULL)
	{
		temp.AddTail(Shapes.GetAt(Selected.GetNext(POS)));
	}
	POS = Selected.GetHeadPosition();
	while (POS != NULL)
	{
		Shapes.RemoveAt(Selected.GetNext(POS));
	}
	CMyShape* Group = new CMyShape(temp);
	Shapes.AddTail(Group);
	POS = Shapes.GetTailPosition();
	Selected.RemoveAll();
	Selected.AddTail(POS);
	Invalidate();
}

// 그룹 해제
void CChildView::OnUngroup()
{
	// CMyShape포인터에서 m_child에 저장된 자손들을 전부 꺼내어 Shapes에 삽입하고
	// 그룹 해제된 CMyShape 포인터는 삭제한다.
	POSITION POS = Selected.GetHeadPosition();
	POSITION CUR_POS;
	if (POS == NULL)
		return;
	while (POS != NULL)
	{
		CUR_POS = POS;
		CMyShape* shape = Shapes.GetAt(Selected.GetNext(POS));
		Shapes.RemoveAt(Selected.GetAt(CUR_POS));
		Selected.RemoveAt(CUR_POS);
		if (shape->hasChild() == false)
		{
			Shapes.AddTail(shape);
			Selected.AddTail(Shapes.GetTailPosition());
			break;
		}
		while (shape->hasChild())
		{
			Shapes.AddTail(shape->getChild());
			Selected.AddTail(Shapes.GetTailPosition());
		}
	}
	Invalidate();
}

void CChildView::OnAlignFront()
{
	// 선택된 도형들을 빼내어 Shapes의 뒤로 다시 삽입한다.
	POSITION POS = Selected.GetHeadPosition();
	POSITION CUR_POS;
	if (POS == NULL)
		return;
	while (POS != NULL)
	{
		CUR_POS = POS;
		Shapes.AddTail(Shapes.GetAt(Selected.GetNext(POS)));
		Selected.AddHead(Shapes.GetTailPosition());
		Shapes.RemoveAt(Selected.GetAt(CUR_POS));
		Selected.RemoveAt(CUR_POS);
	}
	Invalidate();
}

void CChildView::OnAlignBack()
{
	// 선택된 도형들을 빼내어 Shapes의 앞으로 다시 삽입한다.
	POSITION POS = Selected.GetTailPosition();
	POSITION CUR_POS;
	if (POS == NULL)
		return;
	while (POS != NULL)
	{
		CUR_POS = POS;
		Shapes.AddHead(Shapes.GetAt(Selected.GetPrev(POS)));
		Selected.AddTail(Shapes.GetHeadPosition());
		Shapes.RemoveAt(Selected.GetAt(CUR_POS));
		Selected.RemoveAt(CUR_POS);
	}
	Invalidate();
}

// 우클릭 시 콘텍스트 메뉴 열기
void CChildView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_MAINFRAME);

	CMenu* pMenu = menu.GetSubMenu(3);

	pMenu->TrackPopupMenu(
		TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		point.x, point.y, AfxGetMainWnd());
}

// 상태에 따른 메뉴 및 툴바 활성상태, 체크 유무 업데이트
void CChildView::OnUpdateDrawRect(CCmdUI* pCmdUI)
{
	if (mode == RECT)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CChildView::OnUpdateDrawCircle(CCmdUI* pCmdUI)
{
	if (mode == CIRCLE)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CChildView::OnUpdateDrawCurve(CCmdUI* pCmdUI)
{
	if (mode == CURVE)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CChildView::OnUpdateDrawStar(CCmdUI* pCmdUI)
{
	if (mode == STAR)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CChildView::OnUpdateSelect(CCmdUI* pCmdUI)
{
	if (mode == SELECT)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

// 그룹화의 경우 선택된 도형이 2개 이상일 때 활성화
void CChildView::OnUpdateGroup(CCmdUI* pCmdUI)
{
	if (Selected.GetSize() >= 2)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

// 그룹해제, 정렬의 경우 선택된 도형이 1개 이상일 때 활성화
void CChildView::OnUpdateUngroup(CCmdUI* pCmdUI)
{
	if (Selected.GetSize() >= 1)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

void CChildView::OnUpdateAlignFront(CCmdUI* pCmdUI)
{
	if (Selected.GetSize() >= 1)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

void CChildView::OnUpdateAlignBack(CCmdUI* pCmdUI)
{
	if (Selected.GetSize() >= 1)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

// 더블버퍼링에서의 화면 깜빡임 방지
BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	return true;
}