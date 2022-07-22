
// verifierView.h: CverifierView 类的接口
//

#pragma once
#include<vector>
#include "gl/glu.h"
class CverifierView : public CView
{
protected: // 仅从序列化创建
	CverifierView() noexcept;
	DECLARE_DYNCREATE(CverifierView)

	// 特性
public:
	CverifierDoc* GetDocument() const;

	// 操作
public:
	HGLRC  m_hRC;
	HDC m_hDC;
	bool m_moveStart;
	float m_LastX, m_LastY;
	//旋转量
	float m_anglex, m_angley, m_anglez;
	//移动量
	float m_moveX, m_moveY;
	std::vector<GLuint> m_selected_names;
	//相机位置
	int camera_x;
	int camera_y;
	int camera_z;
	//被切割位置
	int cut_position[50] = { 0 };
	//初始旋转角度
	int init_anglex;
	int init_angley;

	void RenderScene();
	void GLInit();//设置像素格式和渲染环境
	void GlRelease();// 清除渲染环境
	void GlResize(int cx, int cy);//处理窗口尺寸的变化
	void GLSetupRC(HDC hdc);//设置渲染环境的光照、颜色和物体的材质等
	void draw_red_cube(int name_point);//绘制默认立方体
	void draw_white_cube(int name_point);//绘制提示性立方体
	void draw_cut_cube(int name_point);//绘制被切割的立方体
	void ProcessSelection(int Xpos, int Ypos);//选取
	void SetCutPosition(int cut_position[]);//随机生成切割位置数组
	BOOL judge();//拼接结果判断
	// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 实现
public:
	virtual ~CverifierView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // verifierView.cpp 中的调试版本
inline CverifierDoc* CverifierView::GetDocument() const
{
	return reinterpret_cast<CverifierDoc*>(m_pDocument);
}
#endif

