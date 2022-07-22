
// verifierView.cpp: CverifierView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "verifier.h"
#endif

#include "verifierDoc.h"
#include "verifierView.h"
#include "gl/gl.h"
#include "gl/glu.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

constexpr auto FACE_1 = 1;
constexpr auto FACE_2 = 2;
constexpr auto FACE_3 = 3;
constexpr auto FACE_4 = 4;
constexpr auto FACE_5 = 5;
constexpr auto FACE_6 = 6;
constexpr auto FACE_7 = 7;
constexpr auto FACE_8 = 8;
constexpr auto FACE_9 = 9;
constexpr auto FACE_10 = 10;
constexpr auto FACE_11 = 11;
constexpr auto FACE_12 = 12;

// CverifierView

IMPLEMENT_DYNCREATE(CverifierView, CView)

BEGIN_MESSAGE_MAP(CverifierView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CverifierView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CverifierView 构造/析构

CverifierView::CverifierView() noexcept
{
	m_moveStart = false;// TODO: 在此处添加构造代码
	m_anglex = 0;
	m_angley = 0;
	m_moveX = 0;
	m_moveY = 0;
	camera_x = 15;
	camera_y = 15;
	camera_z = 35;
	SetCutPosition(cut_position);
	srand(time(0));
	init_anglex = 10 + rand() % 7 * 10;
	init_angley = 10 + rand() % 7 * 10;
}

CverifierView::~CverifierView()
{
}

BOOL CverifierView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CverifierView 绘图

void CverifierView::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int now_drawing_num = 0;//当前在绘制的编号
	int drawn = 0;//已经绘制标记
	int whole_num = 1000;
	while (now_drawing_num < whole_num)//绘制whole_num个立方体
	{
		for (int k = 0; k < 50; k++)//绘制切割出的立方体
		{
			if (now_drawing_num == cut_position[k])
			{
				draw_cut_cube(now_drawing_num);
				drawn = 1;
				break;
			}
			if (now_drawing_num == cut_position[k] - 1)//绘制提示性立方体
			{
				draw_white_cube(now_drawing_num);
				drawn = 1;
				break;
			}
		}
		if (drawn == 0)//绘制默认立方体
		{
			glColor3f(1, 0, 0);
			draw_red_cube(now_drawing_num);

		}
		now_drawing_num++;
		drawn = 0;
	}
}
//设置像素格式和渲染环境
void CverifierView::GLInit()
{
	m_hDC = ::GetDC(m_hWnd);
	static PIXELFORMATDESCRIPTOR pfdWnd = {
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_WINDOW |
	PFD_SUPPORT_OPENGL |
	PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,
	24,
	0,0,0,0,0,
	0,0,0,0,0,0,0,
	32,
	0,0,
	PFD_MAIN_PLANE,
	0,
	0,0,0
	};
	int pixelformat;
	pixelformat = ChoosePixelFormat(m_hDC, &pfdWnd);
	SetPixelFormat(m_hDC, pixelformat, &pfdWnd);
	m_hRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hRC);
	GLSetupRC(m_hDC);
}
// 清除渲染环境
void CverifierView::GlRelease()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_hRC);
	::ReleaseDC(m_hWnd, m_hDC);
}
//处理窗口尺寸的变化
void CverifierView::GlResize(int cx, int cy)
{
	double nRange = 3;
	if (cy == 0) cy = 1;
	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (double)cx / cy, 0.1, 50 * nRange);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera_x, camera_y, camera_z, 0, 0, 0, 0, 1, 0);
}
//设置渲染环境的光照、颜色和物体的材质等
void CverifierView::GLSetupRC(HDC hdc)
{
	//lighting
	GLfloat ambientLight[] = { 0.2f,0.2f,0.2f,1.0f };
	GLfloat diffuseLight[] = { 0.6f,0.6f,0.6f,1.0f };
	GLfloat specular[] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat lightPos[] = { -10.0f,10.0f,10.0f,1.0f };
	//light 0
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);
	//color material
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	//背景色
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//color
	glColor3f(1, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void CverifierView::draw_red_cube(int name_point)
{
	int x = name_point / 100;
	int y = (name_point - x * 100) / 10;
	int z = name_point % 10;
	glInitNames();
	glPushName(0);
	//front face
	glLoadName(FACE_7);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	glVertex3f(x, y, z + 1);
	glVertex3f(x + 1, y, z + 1);
	glVertex3f(x + 1, y + 1, z + 1);
	glVertex3f(x, y + 1, z + 1);
	glEnd();
	// bottom  face
	glLoadName(FACE_8);
	glBegin(GL_POLYGON);
	glNormal3f(0, -1, 0);
	glVertex3f(x, y, z);
	glVertex3f(x + 1, y, z);
	glVertex3f(x + 1, y, z + 1);
	glVertex3f(x, y, z + 1);
	glEnd();
	//right  face
	glLoadName(FACE_9);
	glBegin(GL_POLYGON);
	glNormal3f(1, 0, 0);
	glVertex3f(x + 1, y, z);
	glVertex3f(x + 1, y + 1, z);
	glVertex3f(x + 1, y + 1, z + 1);
	glVertex3f(x + 1, y, z + 1);
	glEnd();
	//left   face
	glLoadName(FACE_10);
	glBegin(GL_POLYGON);
	glNormal3f(-1, 0, 0);
	glVertex3f(x, y, z);
	glVertex3f(x, y, z + 1);
	glVertex3f(x, y + 1, z + 1);
	glVertex3f(x, y + 1, z);
	glEnd();
	//top   face
	glLoadName(FACE_11);
	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	glVertex3f(x, y + 1, z);
	glVertex3f(x, y + 1, z + 1);
	glVertex3f(x + 1, y + 1, z + 1);
	glVertex3f(x + 1, y + 1, z);
	glEnd();
	// back   face
	glLoadName(FACE_12);
	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1);
	glVertex3f(x, y, z);
	glVertex3f(x, y + 1, z);
	glVertex3f(x + 1, y + 1, z);
	glVertex3f(x + 1, y, z);
	glEnd();
	glFlush();
}

void CverifierView::draw_white_cube(int name_point)
{
	int x = name_point / 100;
	int y = (name_point - x * 100) / 10;
	int z = name_point % 10;
	glInitNames();
	glPushName(0);
	//front face
	glColor3f(1, 1, 1);
	glLoadName(FACE_7);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	glVertex3f(x, y, z + 1);
	glVertex3f(x + 1, y, z + 1);
	glVertex3f(x + 1, y + 1, z + 1);
	glVertex3f(x, y + 1, z + 1);
	glEnd();
	// bottom  face
	glColor3f(1, 0, 0);
	glLoadName(FACE_8);
	glBegin(GL_POLYGON);
	glNormal3f(0, -1, 0);
	glVertex3f(x, y, z);
	glVertex3f(x + 1, y, z);
	glVertex3f(x + 1, y, z + 1);
	glVertex3f(x, y, z + 1);
	glEnd();
	//right  face
	glLoadName(FACE_9);
	glBegin(GL_POLYGON);
	glNormal3f(1, 0, 0);
	glVertex3f(x + 1, y, z);
	glVertex3f(x + 1, y + 1, z);
	glVertex3f(x + 1, y + 1, z + 1);
	glVertex3f(x + 1, y, z + 1);
	glEnd();
	//left   face
	glLoadName(FACE_10);
	glBegin(GL_POLYGON);
	glNormal3f(-1, 0, 0);
	glVertex3f(x, y, z);
	glVertex3f(x, y, z + 1);
	glVertex3f(x, y + 1, z + 1);
	glVertex3f(x, y + 1, z);
	glEnd();
	//top   face
	glLoadName(FACE_11);
	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	glVertex3f(x, y + 1, z);
	glVertex3f(x, y + 1, z + 1);
	glVertex3f(x + 1, y + 1, z + 1);
	glVertex3f(x + 1, y + 1, z);
	glEnd();
	// back   face
	glLoadName(FACE_12);
	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1);
	glVertex3f(x, y, z);
	glVertex3f(x, y + 1, z);
	glVertex3f(x + 1, y + 1, z);
	glVertex3f(x + 1, y, z);
	glEnd();
	glFlush();
}

void CverifierView::draw_cut_cube(int name_point)
{
	int r, g, b;
	if (m_selected_names.size() > 0)
	{
		r = 0, g = 0, b = 1;
	}
	else {
		r = 0, g = 1, b = 1;
	}

	int x = name_point / 100;
	int y = (name_point - x * 100) / 10;
	int z = name_point % 10;
	glPushMatrix();
	glRotatef(init_anglex, 0, 1, 0);
	glRotatef(init_angley, 0, 0, 1);
	glTranslated(-15, 0, 0);
	glTranslatef(m_moveX, 0, 0);
	glTranslatef(0, -m_moveY, 0);
	glRotatef(m_angley, 0, 0, 1);
	glRotatef(m_anglex, 0, 1, 0);
	glInitNames();
	glPushName(0);
	//front face
	glLoadName(FACE_1);
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	glVertex3f(x, y, z + 1);
	glVertex3f(x + 1, y, z + 1);
	glVertex3f(x + 1, y + 1, z + 1);
	glVertex3f(x, y + 1, z + 1);
	glEnd();
	// bottom  face
	glLoadName(FACE_2);
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
	glNormal3f(0, -1, 0);
	glVertex3f(x, y, z);
	glVertex3f(x + 1, y, z);
	glVertex3f(x + 1, y, z + 1);
	glVertex3f(x, y, z + 1);
	glEnd();
	//right  face
	glLoadName(FACE_3);
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
	glNormal3f(1, 0, 0);
	glVertex3f(x + 1, y, z);
	glVertex3f(x + 1, y + 1, z);
	glVertex3f(x + 1, y + 1, z + 1);
	glVertex3f(x + 1, y, z + 1);
	glEnd();
	//left   face
	glLoadName(FACE_4);
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
	glNormal3f(-1, 0, 0);
	glVertex3f(x, y, z);
	glVertex3f(x, y, z + 1);
	glVertex3f(x, y + 1, z + 1);
	glVertex3f(x, y + 1, z);
	glEnd();
	//top   face
	glLoadName(FACE_5);
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	glVertex3f(x, y + 1, z);
	glVertex3f(x, y + 1, z + 1);
	glVertex3f(x + 1, y + 1, z + 1);
	glVertex3f(x + 1, y + 1, z);
	glEnd();
	// back   face
	glLoadName(FACE_6);
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1);
	glVertex3f(x, y, z);
	glVertex3f(x, y + 1, z);
	glVertex3f(x + 1, y + 1, z);
	glVertex3f(x + 1, y, z);
	glEnd();
	glPopMatrix();
	glFlush();
}

void CverifierView::ProcessSelection(int Xpos, int Ypos)
{
	GLuint SelectBuffer[64]{};
	GLint hits = 0, viewport[4]{};
	//建立选择缓冲区
	glSelectBuffer(64, SelectBuffer);
	//获得当前视窗信息
	glGetIntegerv(GL_VIEWPORT, viewport);
	CRect rect;
	GetClientRect(rect);
	Ypos = rect.Height() - Ypos;
	//设置投影变换
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	//切换到选择模式
	glRenderMode(GL_SELECT);
	//定义选择视景体
	//根据鼠标位置定义选择矩阵
	//拾取区域的宽和高都是1
	glLoadIdentity();
	gluPickMatrix(Xpos, Ypos, 1, 1, viewport);
	//乘以投影变换的矩阵，该投影矩阵应该和绘制场景时的投影矩阵一致
	//GlResize函数里完成了投影变换
	double nRange = 3;
	float cx = rect.Width();
	float cy = rect.Height();
	gluPerspective(60, (double)cx / cy, 0.1, 50 * nRange);
	//模型视变换
	glMatrixMode(GL_MODELVIEW);
	RenderScene();
	//切换到绘图模式，返回选择缓冲区中的拾取记录数目
	hits = glRenderMode(GL_RENDER);
	//恢复投影变换矩阵
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	//返回模型视矩阵的模式
	glMatrixMode(GL_MODELVIEW);
	m_selected_names.clear();
	if (hits > 0)
	{
		int current = 0;
		for (int i = 0; i < hits; i++)
		{
			int namecount = SelectBuffer[current];
			float zmin = SelectBuffer[current + 1];
			float zmax = SelectBuffer[current + 2];
			current += 3;
			for (int j = 0; j < namecount; j++)
			{
				m_selected_names.push_back(SelectBuffer[current]);
				current++;
			}
		}
	}
}

void CverifierView::SetCutPosition(int cut_position[])
{
	srand(time(0));
	int random_num = rand() % 100;
	cut_position[0] = random_num * 10 + 9;
	int left = 1, right = 2, up = 3, down = 4;
	int now_num = random_num;
	int i = 1;
	while (i < 50) {
		int choice = rand() % 4 + 1;
		if (choice == left) {
			if (now_num < 9) {}
			else {
				cut_position[i] = (now_num - 10) * 10 + 9;
				now_num -= 10;
				i++;
			}
		}
		else if (choice == up) {
			if ((now_num + 1) % 10 == 0) {}
			else {
				cut_position[i] = (now_num + 1) * 10 + 9;
				now_num += 1;
				i++;
			}
		}
		else if (choice == down) {
			if (now_num % 10 == 0) {}
			else
			{
				cut_position[i] = (now_num - 1) * 10 + 9;
				now_num -= 1;
				i++;
			}
		}
		else if (choice == right) {
			if (now_num > 89) {}
			else
			{
				cut_position[i] = (now_num + 10) * 10 + 9;
				now_num += 10;
				i++;
			}
		}
	}
}

BOOL CverifierView::judge()
{
	int tempx = m_anglex, tempy = m_angley;

	while (tempx > 0) {
		tempx -= 360;
	}
	while (tempy > 0) {
		tempy -= 360;
	}
	float distance_anglex = abs(tempx + init_anglex);
	float distance_angley = abs(tempy + init_angley);

	if ((distance_anglex + distance_angley) < 8 && m_moveX < 15.5 && m_moveX>14.5 && m_moveY > -0.5 && m_moveY < 0.5) {
		return true;
	}
	else return false;
}

void CverifierView::OnDraw(CDC* /*pDC*/)
{
	CverifierDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	RenderScene();
	SwapBuffers(m_hDC);
}


// CverifierView 打印


void CverifierView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CverifierView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CverifierView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CverifierView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CverifierView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CverifierView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CverifierView 诊断

#ifdef _DEBUG
void CverifierView::AssertValid() const
{
	CView::AssertValid();
}

void CverifierView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CverifierDoc* CverifierView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CverifierDoc)));
	return (CverifierDoc*)m_pDocument;
}
#endif //_DEBUG


// CverifierView 消息处理程序


int CverifierView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	GLInit();
	return 0;
}


void CverifierView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	wglMakeCurrent(NULL, NULL);
	GlRelease();
}


void CverifierView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	GlResize(cx, cy);
}


BOOL CverifierView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return true;
}


void CverifierView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_selected_names.size() > 0)
	{
		if (nFlags & MK_CONTROL)
		{
			if (!m_moveStart)
			{
				m_LastX = point.x;
				m_LastY = point.y;
				m_moveStart = true;
			}
			else if (nFlags & MK_LBUTTON)
			{
				float yOffset = point.y - m_LastY;
				m_LastY = point.y;
				float xOffset = point.x - m_LastX;
				m_LastX = point.x;
				m_anglex += xOffset / 2.0;
				m_angley += yOffset / 2.0;
				if (m_anglex < 0)
					m_anglex += 360;
				if (m_angley < 0)
					m_angley += 360;
				CRect rect;
				GetClientRect(rect);
				InvalidateRect(rect);
			}
			else;
		}
		if (nFlags & MK_SHIFT)
		{
			if (!m_moveStart)
			{
				m_LastX = point.x;
				m_LastY = point.y;
				m_moveStart = true;
			}
			else if (nFlags & MK_LBUTTON)
			{
				float yOffset = point.y - m_LastY;
				m_LastY = point.y;
				float xOffset = point.x - m_LastX;
				m_LastX = point.x;
				m_moveX += xOffset / 10.0;
				m_moveY += yOffset / 10.0;
				CRect rect;
				GetClientRect(rect);
				InvalidateRect(rect);
			}
			else;
		}
	}
	CView::OnMouseMove(nFlags, point);
}


void CverifierView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!(nFlags & MK_CONTROL))
	{
		ProcessSelection(point.x, point.y);
		CRect rect;
		GetClientRect(rect);
		InvalidateRect(rect);
	}
	CView::OnLButtonDown(nFlags, point);
}


void CverifierView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CRect rect;
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//选中情况下方向键控制角度变换
	if (m_selected_names.size() > 0) {
		switch (nChar)
		{
		case VK_LEFT:
		{
			m_anglex -= 10;
			if (m_anglex < 0)
				m_anglex += 360;
			GetClientRect(rect);
			InvalidateRect(rect);
			break;
		}
		case VK_RIGHT:
		{
			m_anglex += 10;
			if (m_anglex < 0)
				m_anglex += 360;
			GetClientRect(rect);
			InvalidateRect(rect);
			break;
		}
		case VK_UP:
		{
			m_angley += 10;
			if (m_angley < 0)
				m_angley += 360;
			GetClientRect(rect);
			InvalidateRect(rect);
			break;
		}
		case VK_DOWN:
		{
			m_angley -= 10;
			if (m_angley < 0)
				m_angley += 360;
			GetClientRect(rect);
			InvalidateRect(rect);
			break;
		}
		default:
			break;
		}
	}
	//回退
	if (nChar == VK_F2)
	{
		m_moveX = 0;
		m_moveY = 0;
		m_anglex = 0;
		m_angley = 0;
		GetClientRect(rect);
		InvalidateRect(rect);
	}
	//验证
	if (nChar == VK_F1) {
		if (judge()) {
			MessageBox("Pass", "结果", MB_OK);
		}
		else {
			MessageBox("Fail", "结果", MB_OK);
		}
	}
	//重开
	if (nChar == VK_F3) {
		SetCutPosition(cut_position);
		srand(time(0));
		init_anglex = 10 + rand() % 7 * 10;
		init_angley = 10 + rand() % 7 * 10;
		GetClientRect(rect);
		InvalidateRect(rect);
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
