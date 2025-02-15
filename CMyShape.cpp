#include "pch.h"
#include "CMyShape.h"

/////////////////////////////
/// CMyShape ����Լ� ����
/////////////////////////////

// �⺻������
CMyShape::CMyShape()
{
	m_pos = CPoint(0, 0);
	m_color = RGB(255, 255, 255);
}

// ��ġ�� ������ �Ű������� �޴� ������
CMyShape::CMyShape(CPoint pos, COLORREF color)
{
	m_pos = pos;
	m_color = color;
}

// ���õ� ��ü���� CMyShape ������ ����Ʈ�� ���·� �Ű������� �޴� ������
CMyShape::CMyShape(CList <CMyShape*>& Selected) : CMyShape()
{
	m_box = Selected.GetHead()->m_box;
	POSITION POS = Selected.GetHeadPosition();
	while (POS != NULL)
	{
		// ���� ǥ�� �ڽ� ������ �������� ���� �����ϵ��� �������Ѵ�
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

// �ڼտ� ���� �������� �׸��� �ϴ� �Լ� 
void CMyShape::draw(CDC& drawDC)
{
	POSITION POS = m_child.GetHeadPosition();
	while (POS != NULL)
	{
		m_child.GetNext(POS)->draw(drawDC);
	}
}

// ���� ǥ�� �ڽ��� �׸��� �Լ�
void CMyShape::drawBox(CDC& drawDC)
{
	CPen pen(PS_DOT, 1, RGB(255, 0, 0));
	drawDC.SelectStockObject(NULL_BRUSH);
	drawDC.SelectObject(&pen);
	drawDC.Rectangle(m_box);
}

// �ڼ��� ��� �������� �����̰�, ���� ǥ�� �ڽ��� �Բ� �̵���Ű�� �Լ�
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

// �Ű������� ���� ����Ʈ�� �ڼտ� �����ִ� ������ ���ο� �ִ��� �Ǻ��ϴ� �Լ�
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

// �巡�� ���� ���� �ȿ� �ڽ��� �ִ��� �Ǻ��ϴ� �Լ�
bool CMyShape::Is_in_box(CRect rect)
{
	if (m_box.left > rect.left && m_box.right < rect.right && m_box.top > rect.top && m_box.bottom < rect.bottom)
		return true;
	else
		return false;
}

// �ڼ��� �տ��� �ϳ� ������ ��ȯ�ϴ� �Լ�
CMyShape* CMyShape::getChild()
{
	CMyShape* temp = m_child.GetHead();
	m_child.RemoveHead();
	return temp;
}
// ��ġ�� ������Ʈ �ϴ� �Լ�
void CMyShape::setPos(CPoint pos) { m_pos = pos; }
// �ڼ� ���� ���θ� Ȯ���ϴ� �Լ�
bool CMyShape::hasChild() { return !m_child.IsEmpty(); }
// ������ ��ġ�� ��ȯ�ϴ� �Լ�
CPoint CMyShape::getPos() { return m_pos; }
// ������ �÷��� ��ȯ�ϴ� �Լ�
COLORREF CMyShape::getColor() { return m_color; }

//////////////////////////////////////
///
/// CMyRect ������ ���� �� �Լ� ������
/// 
/////////////////////////////////////

// CMyRect�� ������, �ʺ�� ���̸� �߰������� �䱸�Ѵ�
CMyRect::CMyRect(CPoint pos, COLORREF color, int width, int height) : CMyShape(pos, color)
{
	m_w = width;
	m_h = height;
	// ���� ǥ�� �ڽ� ����
	m_box.left = pos.x - m_w / 2 - 5;
	m_box.right = pos.x + m_w / 2 + 5;
	m_box.top = pos.y - m_h / 2 - 5;
	m_box.bottom = pos.y + m_h / 2 + 5;
}

// ��ο� ���ؽ�Ʈ�� �޾� �簢���� �׸��� �Լ�, CMyShape::draw�Լ��� �������̴�
void CMyRect::draw(CDC& drawDC)
{
	CPoint p = getPos();
	CBrush brush(getColor());
	CPen pen(PS_SOLID, 3, RGB(0, 0, 0));
	drawDC.SelectObject(&brush);
	drawDC.SelectObject(&pen);
	drawDC.Rectangle(p.x - m_w / 2, p.y - m_h / 2, p.x + m_w / 2, p.y + m_h / 2);
}

// ��ο� ���ؽ�Ʈ�� �޾� ���� ǥ�� �ڽ��� �׸��� �Լ�, CMyShape::drawBox�Լ��� �������̴�
void CMyRect::drawBox(CDC& drawDC)
{
	CPen pen(PS_DOT, 1, RGB(255, 0, 0));
	drawDC.SelectStockObject(NULL_BRUSH);
	drawDC.SelectObject(&pen);
	drawDC.Rectangle(m_box);
}

// x�� y�� ��ȭ���� �޾� ��ü�� ���� ǥ�� �ڽ��� �̵���Ű�� �Լ� CMyShape::move �Լ��� �������̴�
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

// ����Ʈ p�� �簢�� �ȿ� �����ִ��� ���θ� ��ȯ�ϴ� �Լ�, CMyShape::Is_in �Լ��� �������̴�
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
/// CMyCircle ������ ���� �� �Լ� ������
/// 
/////////////////////////////////////

// CMyCircle�� ������, �������� �߰������� �䱸�Ѵ�
CMyCircle::CMyCircle(CPoint pos, COLORREF color, int radius) : CMyShape(pos, color)
{
	m_r = radius;
	// �������� �̿��� ���� ǥ�� �ڽ� ����
	m_box.left = pos.x - radius - 5;
	m_box.right = pos.x + radius + 5;
	m_box.top = pos.y - radius - 5;
	m_box.bottom = pos.y + radius + 5;
}

// ��ο� ���ؽ�Ʈ�� �޾� ���� �׸��� �Լ�, CMyShape::draw�Լ��� �������̴�
void CMyCircle::draw(CDC& drawDC)
{
	CPoint p = getPos();
	CBrush brush(getColor());
	CPen pen(PS_SOLID, 3, RGB(0, 0, 0));
	drawDC.SelectObject(&brush);
	drawDC.SelectObject(&pen);
	drawDC.Ellipse(p.x - m_r, p.y - m_r, p.x + m_r, p.y + m_r);
}

// ��ο� ���ؽ�Ʈ�� �޾� ���� ǥ�� �ڽ��� �׸��� �Լ�, CMyShape::drawBox�Լ��� �������̴�
void CMyCircle::drawBox(CDC& drawDC)
{
	CPen pen(PS_DOT, 1, RGB(255, 0, 0));
	drawDC.SelectStockObject(NULL_BRUSH);
	drawDC.SelectObject(&pen);
	drawDC.Rectangle(m_box);
}

// x�� y�� ��ȭ���� �޾� ��ü�� ���� ǥ�� �ڽ��� �̵���Ű�� �Լ� CMyShape::move �Լ��� �������̴�
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

// ����Ʈ p�� �� �ȿ� �����ִ��� ���θ� ��ȯ�ϴ� �Լ�, CMyShape::Is_in �Լ��� �������̴�
bool CMyCircle::Is_in(CPoint p)
{
	CPoint pos = getPos();
	// ���� ���� �߽� ������ �Ÿ��� �̿��� �Ǻ��Ѵ�
	float dist = sqrt((p.x - pos.x) * (p.x - pos.x) + (p.y - pos.y) * (p.y - pos.y));
	if (dist < m_r)
		return true;
	else
		return false;
}

/////////////////////////////////////////
///									  ///
/// CMyLine ������ ���� �� �Լ� ������  ///
///									  ///
/////////////////////////////////////////

// CMyLine�� ������, CPoint ����Ʈ�� �䱸�Ѵ�
CMyLine::CMyLine(CPoint pos, COLORREF color, CList<CPoint>& pts) : CMyShape(pos, color)
{
	CPoint p = pts.GetHead();
	m_box = CRect(p.x, p.y, p.x, p.y);
	POSITION POS = pts.GetHeadPosition();
	// ������ ���� ������ �� �ִ� ���� ǥ�� �ڽ� ����
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

// ��ο� ���ؽ�Ʈ�� �޾� ��� �׸��� �Լ�, CMyShape::draw�Լ��� �������̴�
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

// ��ο� ���ؽ�Ʈ�� �޾� ���� ǥ�� �ڽ��� �׸��� �Լ�, CMyShape::drawBox�Լ��� �������̴�
void CMyLine::drawBox(CDC& drawDC)
{
	CPen pen(PS_DOT, 1, RGB(255, 0, 0));
	drawDC.SelectStockObject(NULL_BRUSH);
	drawDC.SelectObject(&pen);
	drawDC.Rectangle(m_box);
}

// x�� y�� ��ȭ���� �޾� ��ü�� ���� ǥ�� �ڽ��� �̵���Ű�� �Լ� CMyShape::move �Լ��� �������̴�
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

// ����Ʈ p�� �� ��ó�� �ִ��� ���θ� ��ȯ�ϴ� �Լ�, CMyShape::Is_in �Լ��� �������̴�
// ���� ��� ���� Ŭ���Ǵ� ������ �а� �Ͽ� Ŭ���� ��Ȱ�ϵ��� �ǵ��ߴ�
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
/// CMyStar ������ ���� �� �Լ� ������
/// 
/////////////////////////////////////

// CMyLine�� ������, ũ�Ⱑ 10�� CPoint �迭�� �䱸�Ѵ�
CMyStar::CMyStar(CPoint pos, COLORREF color, CPoint* pts) : CMyShape(pos, color)
{
	for (int i = 0; i < 10; i++)
		m_pts[i] = pts[i];
	// �������� ��ǥ�� �̿��� ���� ǥ�� �ڽ� ����
	m_box.left = pts[8].x - 5;
	m_box.right = pts[2].x + 5;
	m_box.top = pts[0].y - 5;
	m_box.bottom = pts[4].y + 5;
}

// ��ο� ���ؽ�Ʈ�� �޾� ���� �׸��� �Լ�, CMyShape::draw�Լ��� �������̴�
void CMyStar::draw(CDC& drawDC)
{
	CPoint p = getPos();
	CBrush brush(getColor());
	CBrush black(RGB(0, 0, 0));
	drawDC.SelectObject(&brush);
	// ũ�Ⱑ 10�� ����Ʈ �迭�� �̿��� �ٰ����� ����� ����Ѵ�
	CRgn rgn;
	rgn.CreatePolygonRgn(m_pts, 10, ALTERNATE);
	drawDC.PaintRgn(&rgn);
	drawDC.FrameRgn(&rgn, &black, 3, 3);
}

// ��ο� ���ؽ�Ʈ�� �޾� ���� ǥ�� �ڽ��� �׸��� �Լ�, CMyShape::drawBox�Լ��� �������̴�
void CMyStar::drawBox(CDC& drawDC)
{
	CPen pen(PS_DOT, 1, RGB(255, 0, 0));
	drawDC.SelectStockObject(NULL_BRUSH);
	drawDC.SelectObject(&pen);
	drawDC.Rectangle(m_box);
}

// x�� y�� ��ȭ���� �޾� ��ü�� ���� ǥ�� �ڽ��� �̵���Ű�� �Լ� CMyShape::move �Լ��� �������̴�
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

// ����Ʈ p�� �� �ȿ� �ִ��� ���θ� ��ȯ�ϴ� �Լ�, CMyShape::Is_in �Լ��� �������̴�
bool CMyStar::Is_in(CPoint p)
{
	CRgn rgn;
	rgn.CreatePolygonRgn(m_pts, 10, ALTERNATE);
	// rgn������ ���� �Ǵ��ϴ� PtInRegion �Լ��� Ȱ���Ͽ���.
	return rgn.PtInRegion(p);
}