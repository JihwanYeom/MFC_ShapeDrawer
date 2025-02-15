
// ChildView.h: CChildView 클래스의 인터페이스
//


#pragma once
#include "CMyShape.h"


// CChildView 창

class CChildView : public CWnd
{
// 생성입니다.
public:
	CChildView();

// 특성입니다.
public:
	CList <CMyShape*> Shapes; // 도형들을 업캐스팅 하여 저장하는 CMyShape 포인터 리스트
	CList <POSITION> Selected; // 선택된 도형들의 POSITION을 저장하는 리스트
	CPoint p1, p2; // 도형을 그리는데 사용할 점 p1, p2
	CList <CPoint> pts; // 곡선을 그리는데 사용할 포인트 리스트 pts
	CPoint star[10]; // 별을 그리는데 사용할 포인트 배열 star
	COLORREF color; // 현재 색상을 저장할 변수 color
	int radius; // 원, 별을 그리는데 사용할 반지름 radius
	bool dragSelect; // 드래그 선택 모드 여부를 나타내는 dragSelect 변수
	enum Mode { RECT, CIRCLE, CURVE, STAR, SELECT }; // 모드들을 나타내는 열거형 Mode
	Mode mode; // 현재 모드를 저장할 mode 변수
// 작업입니다.
public:

// 재정의입니다.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildView();

	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDrawRect();
	afx_msg void OnDrawCircle();
	afx_msg void OnDrawCurve();
	afx_msg void OnDrawStar();
	afx_msg void OnSelect();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnGroup();
	afx_msg void OnUngroup();
	afx_msg void OnUpdateDrawRect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawCircle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawCurve(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawStar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSelect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGroup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUngroup(CCmdUI* pCmdUI);
	afx_msg void OnAlignFront();
	afx_msg void OnAlignBack();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnUpdateAlignFront(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAlignBack(CCmdUI* pCmdUI);
};

