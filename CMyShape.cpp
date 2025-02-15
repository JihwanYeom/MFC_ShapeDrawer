#include "pch.h"
#include "CMyShape.h"

/////////////////////////////
/// CMyShape 멤버함수 정의
/////////////////////////////

// 기본생성자
CMyShape::CMyShape()
{
	m_pos = CPoint(0, 0);
	m_color = RGB(255, 255, 255);
}

// 위치와 색상을 매개변수로 받는 생성자
CMyShape::CMyShape(CPoint pos, COLORREF color)
{
	m_pos = pos;
	m_color = color;
}

// 선택된 개체들을 CMyShape 포인터 리스트의 형태로 매개변수로 받는 생성자
CMyShape::CMyShape(CList <CMyShape*>& Selected) : CMyShape()
{
	m_box = Selected.GetHead()->m_box;
	POSITION POS = Selected.GetHeadPosition();
	while (POS != NULL)
	{
		// 선택 표시 박스 영역이 도형들을 전부 포함하도록 재정의한다
		CMyShape* Shape = Selected.GetNext(POS);
		if (Shape->m_box.left < m_box.left)
			m_box.left = Shape->m_box.left;
		if (Shape->m_box.right > m_box.right)
			m_box.right = Shape->m_box.right;
		if (Shape->m_box.top < m_box.top)
			m_box.top = Shape->m_box.top;
		if (Shape->m_box.bottom > m_box.bottom)
			m_box.bottom = Shape->m_box.bottom;
		m_child.AddTail(Shape);
	}
}

// 자손에 속한 도형들을 그리게 하는 함수 
void CMyShape::draw(CDC& drawDC)
{
	POSITION POS = m_child.GetHeadPosition();
	while (POS != NULL)
	{
		m_child.GetNext(POS)->draw(drawDC);
	}
}

// 선택 표시 박스를 그리는 함수
void CMyShape::drawBox(CDC& drawDC)
{
	CPen pen(PS_DOT, 1, RGB(255, 0, 0));
	drawDC.SelectStockObject(NULL_BRUSH);
	drawDC.SelectObject(&pen);
	drawDC.Rectangle(m_box);
}

// 자손의 모든 도형들을 움직이고, 선택 표시 박스를 함께 이동시키는 함수
void CMyShape::move(int dx, int dy)
{
	POSITION POS = m_child.GetHeadPosition();
	while (POS != NULL)
	{
		CMyShape* Shape = m_child.GetNext(POS);
		Shape->move(dx, dy);

	}
	m_box.left += dx;
	m_box.right += dx;
	m_box.top += dy;
	m_box.bottom += dy;
}

// 매개변수로 받은 포인트가 자손에 속해있는 도형들 내부에 있는지 판별하는 함수
bool CMyShape::Is_in(CPoint p)
{
	POSITION POS = m_child.GetHeadPosition();
	while (POS != NULL)
	{
		if (m_child.GetNext(POS)->Is_in(p) == true)
			return true;
	}
	return false;
}

// 드래그 선택 영역 안에 박스가 있는지 판별하는 함수
bool CMyShape::Is_in_box(CRect rect)
{
	if (m_box.left > rect.left && m_box.right < rect.right && m_box.top > rect.top && m_box.bottom < rect.bottom)
		return true;
	else
		return false;
}

// 자손을 앞에서 하나 빼내서 반환하는 함수
CMyShape* CMyShape::getChild()
{
	CMyShape* temp = m_child.GetHead();
	m_child.RemoveHead();
	return temp;
}
// 위치를 업데이트 하는 함수
void CMyShape::setPos(CPoint pos) { m_pos = pos; }
// 자손 보유 여부를 확인하는 함수
bool CMyShape::hasChild() { return !m_child.IsEmpty(); }
// 도형의 위치를 반환하는 함수
CPoint CMyShape::getPos() { return m_pos; }
// 도형의 컬러를 반환하는 함수
COLORREF CMyShape::getColor() { return m_color; }

//////////////////////////////////////
///
/// CMyRect 생성자 정의 및 함수 재정의
/// 
/////////////////////////////////////

// CMyRect의 생성자, 너비와 높이를 추가적으로 요구한다
CMyRect::CMyRect(CPoint pos, COLORREF color, int width, int height) : CMyShape(pos, color)
{
	m_w = width;
	m_h = height;
	// 선택 표시 박스 정의
	m_box.left = pos.x - m_w / 2 - 5;
	m_box.right = pos.x + m_w / 2 + 5;
	m_box.top = pos.y - m_h / 2 - 5;
	m_box.bottom = pos.y + m_h / 2 + 5;
}

// 드로우 컨텍스트를 받아 사각형을 그리는 함수, CMyShape::draw함수의 재정의이다
void CMyRect::draw(CDC& drawDC)
{
	CPoint p = getPos();
	CBrush brush(getColor());
	CPen pen(PS_SOLID, 3, RGB(0, 0, 0));
	drawDC.SelectObject(&brush);
	drawDC.SelectObject(&pen);
	drawDC.Rectangle(p.x - m_w / 2, p.y - m_h / 2, p.x + m_w / 2, p.y + m_h / 2);
}

// 드로우 컨텍스트를 받아 선택 표시 박스를 그리는 함수, CMyShape::drawBox함수의 재정의이다
void CMyRect::drawBox(CDC& drawDC)
{
	CPen pen(PS_DOT, 1, RGB(255, 0, 0));
	drawDC.SelectStockObject(NULL_BRUSH);
	drawDC.SelectObject(&pen);
	drawDC.Rectangle(m_box);
}

// x와 y의 변화량을 받아 객체와 선택 표시 박스를 이동시키는 함수 CMyShape::move 함수의 재정의이다
void CMyRect::move(int dx, int dy)
{
	CPoint pos = getPos();
	pos.x += dx;
	pos.y += dy;
	setPos(pos);
	m_box.left = pos.x - m_w / 2 - 5;
	m_box.right = pos.x + m_w / 2 + 5;
	m_box.top = pos.y - m_h / 2 - 5;
	m_box.bottom = pos.y + m_h / 2 + 5;
}

// 포인트 p가 사각형 안에 속해있는지 여부를 반환하는 함수, CMyShape::Is_in 함수의 재정의이다
bool CMyRect::Is_in(CPoint p)
{
	CPoint pos = getPos();
	if (p.x > pos.x - m_w / 2 && p.x < pos.x + m_w / 2 && p.y > pos.y - m_h / 2 && p.y < pos.y + m_h / 2)
		return true;
	else
		return false;
}

//////////////////////////////////////
///
/// CMyCircle 생성자 정의 및 함수 재정의
/// 
/////////////////////////////////////

// CMyCircle의 생성자, 반지름을 추가적으로 요구한다
CMyCircle::CMyCircle(CPoint pos, COLORREF color, int radius) : CMyShape(pos, color)
{
	m_r = radius;
	// 반지름을 이용해 선택 표시 박스 정의
	m_box.left = pos.x - radius - 5;
	m_box.right = pos.x + radius + 5;
	m_box.top = pos.y - radius - 5;
	m_box.bottom = pos.y + radius + 5;
}

// 드로우 컨텍스트를 받아 원을 그리는 함수, CMyShape::draw함수의 재정의이다
void CMyCircle::draw(CDC& drawDC)
{
	CPoint p = getPos();
	CBrush brush(getColor());
	CPen pen(PS_SOLID, 3, RGB(0, 0, 0));
	drawDC.SelectObject(&brush);
	drawDC.SelectObject(&pen);
	drawDC.Ellipse(p.x - m_r, p.y - m_r, p.x + m_r, p.y + m_r);
}

// 드로우 컨텍스트를 받아 선택 표시 박스를 그리는 함수, CMyShape::drawBox함수의 재정의이다
void CMyCircle::drawBox(CDC& drawDC)
{
	CPen pen(PS_DOT, 1, RGB(255, 0, 0));
	drawDC.SelectStockObject(NULL_BRUSH);
	drawDC.SelectObject(&pen);
	drawDC.Rectangle(m_box);
}

// x와 y의 변화량을 받아 객체와 선택 표시 박스를 이동시키는 함수 CMyShape::move 함수의 재정의이다
void CMyCircle::move(int dx, int dy)
{
	CPoint pos = getPos();
	pos.x += dx;
	pos.y += dy;
	setPos(pos);
	m_box.left = pos.x - m_r - 5;
	m_box.right = pos.x + m_r + 5;
	m_box.top = pos.y - m_r - 5;
	m_box.bottom = pos.y + m_r + 5;
}

// 포인트 p가 원 안에 속해있는지 여부를 반환하는 함수, CMyShape::Is_in 함수의 재정의이다
bool CMyCircle::Is_in(CPoint p)
{
	CPoint pos = getPos();
	// 점과 원의 중심 사이의 거리를 이용해 판별한다
	float dist = sqrt((p.x - pos.x) * (p.x - pos.x) + (p.y - pos.y) * (p.y - pos.y));
	if (dist < m_r)
		return true;
	else
		return false;
}

/////////////////////////////////////////
///									  ///
/// CMyLine 생성자 정의 및 함수 재정의  ///
///									  ///
/////////////////////////////////////////

// CMyLine의 생성자, CPoint 리스트를 요구한다
CMyLine::CMyLine(CPoint pos, COLORREF color, CList<CPoint>& pts) : CMyShape(pos, color)
{
	CPoint p = pts.GetHead();
	m_box = CRect(p.x, p.y, p.x, p.y);
	POSITION POS = pts.GetHeadPosition();
	// 점들을 전부 포함할 수 있는 선택 표시 박스 정의
	while (POS != NULL)
	{
		p = pts.GetNext(POS);
		m_pts.AddTail(p);
		if (p.x < m_box.left)
			m_box.left = p.x;
		if (p.x > m_box.right)
			m_box.right = p.x;
		if (p.y < m_box.top)
			m_box.top = p.y;
		if (p.y > m_box.bottom)
			m_box.bottom = p.y;
	}
	m_box.top -= 5;
	m_box.bottom += 5;
	m_box.left -= 5;
	m_box.right += 5;
}

// 드로우 컨텍스트를 받아 곡선을 그리는 함수, CMyShape::draw함수의 재정의이다
void CMyLine::draw(CDC& drawDC)
{
	CPen pen(PS_SOLID, 3, getColor());
	drawDC.SelectObject(&pen);
	POSITION POS1 = m_pts.GetHeadPosition();
	m_pts.GetNext(POS1);
	POSITION POS2 = m_pts.GetHeadPosition();
	while (POS1 != NULL)
	{
		drawDC.MoveTo(m_pts.GetNext(POS2));
		drawDC.LineTo(m_pts.GetNext(POS1));
	}
}

// 드로우 컨텍스트를 받아 선택 표시 박스를 그리는 함수, CMyShape::drawBox함수의 재정의이다
void CMyLine::drawBox(CDC& drawDC)
{
	CPen pen(PS_DOT, 1, RGB(255, 0, 0));
	drawDC.SelectStockObject(NULL_BRUSH);
	drawDC.SelectObject(&pen);
	drawDC.Rectangle(m_box);
}

// x와 y의 변화량을 받아 객체와 선택 표시 박스를 이동시키는 함수 CMyShape::move 함수의 재정의이다
void CMyLine::move(int dx, int dy)
{
	CPoint p = m_pts.GetHead();
	p.x += dx;
	p.y += dy;
	m_box = CRect(p.x, p.y, p.x, p.y);
	POSITION POS = m_pts.GetHeadPosition();
	while (POS != NULL)
	{
		CPoint p = m_pts.GetAt(POS);
		p.x += dx;
		p.y += dy;
		if (p.x < m_box.left)
			m_box.left = p.x;
		if (p.x > m_box.right)
			m_box.right = p.x;
		if (p.y < m_box.top)
			m_box.top = p.y;
		if (p.y > m_box.bottom)
			m_box.bottom = p.y;
		m_pts.SetAt(POS, p);
		m_pts.GetNext(POS);
	}
}

// 포인트 p가 선 근처에 있는지 여부를 반환하는 함수, CMyShape::Is_in 함수의 재정의이다
// 선의 경우 점이 클릭되는 범위를 넓게 하여 클릭이 원활하도록 의도했다
bool CMyLine::Is_in(CPoint p)
{
	POSITION POS = m_pts.GetHeadPosition();
	while (POS != NULL)
	{
		CPoint q = m_pts.GetNext(POS);
		if (p.x > q.x - 10 && p.x < q.x + 10 && p.y > q.y - 10 && p.y < q.y + 10)
			return true;
	}
	return false;
}

//////////////////////////////////////
///
/// CMyStar 생성자 정의 및 함수 재정의
/// 
/////////////////////////////////////

// CMyLine의 생성자, 크기가 10인 CPoint 배열를 요구한다
CMyStar::CMyStar(CPoint pos, COLORREF color, CPoint* pts) : CMyShape(pos, color)
{
	for (int i = 0; i < 10; i++)
		m_pts[i] = pts[i];
	// 꼭짓점의 좌표를 이용해 선택 표시 박스 정의
	m_box.left = pts[8].x - 5;
	m_box.right = pts[2].x + 5;
	m_box.top = pts[0].y - 5;
	m_box.bottom = pts[4].y + 5;
}

// 드로우 컨텍스트를 받아 별을 그리는 함수, CMyShape::draw함수의 재정의이다
void CMyStar::draw(CDC& drawDC)
{
	CPoint p = getPos();
	CBrush brush(getColor());
	CBrush black(RGB(0, 0, 0));
	drawDC.SelectObject(&brush);
	// 크기가 10인 포인트 배열을 이용해 다각형을 만들어 출력한다
	CRgn rgn;
	rgn.CreatePolygonRgn(m_pts, 10, ALTERNATE);
	drawDC.PaintRgn(&rgn);
	drawDC.FrameRgn(&rgn, &black, 3, 3);
}

// 드로우 컨텍스트를 받아 선택 표시 박스를 그리는 함수, CMyShape::drawBox함수의 재정의이다
void CMyStar::drawBox(CDC& drawDC)
{
	CPen pen(PS_DOT, 1, RGB(255, 0, 0));
	drawDC.SelectStockObject(NULL_BRUSH);
	drawDC.SelectObject(&pen);
	drawDC.Rectangle(m_box);
}

// x와 y의 변화량을 받아 객체와 선택 표시 박스를 이동시키는 함수 CMyShape::move 함수의 재정의이다
void CMyStar::move(int dx, int dy)
{
	for (int i = 0; i < 10; i++)
	{
		m_pts[i].x += dx;
		m_pts[i].y += dy;
	}
	m_box.left = m_pts[8].x - 5;
	m_box.right = m_pts[2].x + 5;
	m_box.top = m_pts[0].y - 5;
	m_box.bottom = m_pts[4].y + 5;
}

// 포인트 p가 별 안에 있는지 여부를 반환하는 함수, CMyShape::Is_in 함수의 재정의이다
bool CMyStar::Is_in(CPoint p)
{
	CRgn rgn;
	rgn.CreatePolygonRgn(m_pts, 10, ALTERNATE);
	// rgn내부의 점을 판단하는 PtInRegion 함수를 활용하였다.
	return rgn.PtInRegion(p);
}