#ifndef _C_MY_CLASS_
#define _C_MY_CLASS_

// CMyShape Ŭ���� ����
class CMyShape
{
private:
	CPoint m_pos; // ������ ��ġ
	COLORREF m_color; // ������ ����
	CList <CMyShape*> m_child; // �ڼ����� ������ �ִ� ����
protected:
	CRect m_box; // ���� ǥ�� �ڽ�
public:
	CMyShape();
	CMyShape(CPoint pos, COLORREF color);
	CMyShape(CList <CMyShape*>& Selected);
	// draw, drawBox, move, Is_in�� �����Լ��� �����Ͽ� �������Ͽ� �����
	virtual void draw(CDC& drawDC);
	virtual void drawBox(CDC& drawDC);
	virtual void move(int dx, int dy);
	virtual bool Is_in(CPoint p);
	bool Is_in_box(CRect rect);
	bool hasChild();
	void setPos(CPoint pos);
	CMyShape* getChild();
	CPoint getPos();
	COLORREF getColor();
};

// CMyRect Ŭ���� ����
class CMyRect : public CMyShape
{
private:
	int m_w, m_h;
public:
	CMyRect(CPoint pos, COLORREF color, int width, int height);
	// draw, drawBox, move, Is_in�� �������Ͽ� �����
	virtual void draw(CDC& drawDC);
	virtual void drawBox(CDC& drawDC);
	virtual void move(int dx, int dy);
	virtual bool Is_in(CPoint p);
};

// CMyCircle Ŭ���� ����
class CMyCircle : public CMyShape
{
private:
	int m_r;
public:
	CMyCircle(CPoint pos, COLORREF color, int radius);
	// draw, drawBox, move, Is_in�� �������Ͽ� �����
	virtual void draw(CDC& drawDC);
	virtual void drawBox(CDC& drawDC);
	virtual void move(int dx, int dy);
	virtual bool Is_in(CPoint p);
};

// CMyLine Ŭ���� ����
class CMyLine : public CMyShape
{
private:
	CList <CPoint> m_pts;
public:
	CMyLine(CPoint pos, COLORREF color, CList<CPoint>& pts);
	// draw, drawBox, move, Is_in�� �������Ͽ� �����
	virtual void draw(CDC& drawDC);
	virtual void drawBox(CDC& drawDC);
	virtual void move(int dx, int dy);
	virtual bool Is_in(CPoint p);
};

// CMyStar Ŭ���� ����
class CMyStar : public CMyShape
{
private:
	CPoint m_pts[10];
public:
	CMyStar(CPoint pos, COLORREF color, CPoint* pts);
	// draw, drawBox, move, Is_in�� �������Ͽ� �����
	virtual void draw(CDC& drawDC);
	virtual void drawBox(CDC& drawDC);
	virtual void move(int dx, int dy);
	virtual bool Is_in(CPoint p);
};

#endif