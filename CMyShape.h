#ifndef _C_MY_CLASS_
#define _C_MY_CLASS_

// CMyShape 클래스 선언
class CMyShape
{
private:
	CPoint m_pos; // 도형의 위치
	COLORREF m_color; // 도형의 색상
	CList <CMyShape*> m_child; // 자손으로 가지고 있는 도형
protected:
	CRect m_box; // 선택 표시 박스
public:
	CMyShape();
	CMyShape(CPoint pos, COLORREF color);
	CMyShape(CList <CMyShape*>& Selected);
	// draw, drawBox, move, Is_in은 가상함수로 선언하여 재정의하여 사용함
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

// CMyRect 클래스 선언
class CMyRect : public CMyShape
{
private:
	int m_w, m_h;
public:
	CMyRect(CPoint pos, COLORREF color, int width, int height);
	// draw, drawBox, move, Is_in을 재정의하여 사용함
	virtual void draw(CDC& drawDC);
	virtual void drawBox(CDC& drawDC);
	virtual void move(int dx, int dy);
	virtual bool Is_in(CPoint p);
};

// CMyCircle 클래스 선언
class CMyCircle : public CMyShape
{
private:
	int m_r;
public:
	CMyCircle(CPoint pos, COLORREF color, int radius);
	// draw, drawBox, move, Is_in을 재정의하여 사용함
	virtual void draw(CDC& drawDC);
	virtual void drawBox(CDC& drawDC);
	virtual void move(int dx, int dy);
	virtual bool Is_in(CPoint p);
};

// CMyLine 클래스 선언
class CMyLine : public CMyShape
{
private:
	CList <CPoint> m_pts;
public:
	CMyLine(CPoint pos, COLORREF color, CList<CPoint>& pts);
	// draw, drawBox, move, Is_in을 재정의하여 사용함
	virtual void draw(CDC& drawDC);
	virtual void drawBox(CDC& drawDC);
	virtual void move(int dx, int dy);
	virtual bool Is_in(CPoint p);
};

// CMyStar 클래스 선언
class CMyStar : public CMyShape
{
private:
	CPoint m_pts[10];
public:
	CMyStar(CPoint pos, COLORREF color, CPoint* pts);
	// draw, drawBox, move, Is_in을 재정의하여 사용함
	virtual void draw(CDC& drawDC);
	virtual void drawBox(CDC& drawDC);
	virtual void move(int dx, int dy);
	virtual bool Is_in(CPoint p);
};

#endif