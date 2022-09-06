#include<unordered_map>
#include<windows.h>
#include<windowsx.h>
#include<math.h>
#include<stdio.h>
#include <string>
#include <string.h>
#include<typeinfo>
#include <io.h>  
#include <iostream>
#include <vector>
//动态库调用

#include "F:/工作/画板/Shape/Shape.h"
#pragma comment(lib,"F:/工作/画板/Shape/Debug/SHAPE.lib")

using namespace std;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



//参数定义
Shape* shape[100];
ShapePackage* shapePackage[10];
int kindnumber = 0;
int shapenum = 0;  //图形数量

vector<string> files;

Builder* builder;
Painter* painter;

//typedef void(*FUNCSP)();
//typedef ShapePackage* (*FUNGSP)(int i);
//HINSTANCE hActive = LoadLibrary(TEXT("F:/工作/画板/MyFuctionDLL/Debug/MyFuctionDLL.dll"));
//FUNCSP createShapePackage = (FUNCSP)GetProcAddress(hActive, "CreateShapePackage");
//FUNGSP getShapePackage = (FUNGSP)GetProcAddress(hActive, "GetShapePackage");

char* savePath = "F:/工作/画板/newhuaban/Debug/Save/save01.txt";  //自己设置目录  exe文件有时执行不了是因为exe文件路径和cpp文件路径不同，所以需要绝对路径

void InitShapePackage() {
	//char* filePath = "./Debug/MyDll";//自己设置目录  
	char* filePath = "F:/工作/画板/newhuaban/Debug/MyDll";//自己设置目录  exe文件有时执行不了是因为exe文件路径和cpp文件路径不同，所以需要绝对路径
	long   hFile = 0;
	//文件信息，声明一个存储文件信息的结构体  
	struct _finddata_t fileinfo;
	string p;//字符串，存放路径
	if ((hFile = _findfirst(p.assign(filePath).append("\\*.dll").c_str(), &fileinfo)) != -1)//若查找成功，则进入
	{
		do
		{
			files.push_back(p.assign(filePath).append("\\").append(fileinfo.name));
		} while (_findnext(hFile, &fileinfo) == 0);
		//_findclose函数结束查找
		_findclose(hFile);
	}
	kindnumber = files.size();
	typedef ShapePackage* (*FUNGSP)();
	HINSTANCE hShape;
	FUNGSP GetPackage;
	for (int i = 0; i < files.size(); i++)
	{
		hShape = LoadLibrary(files[i].c_str());
		GetPackage = (FUNGSP)GetProcAddress(hShape, "GetPackage");
		shapePackage[i] = GetPackage();
		//shapePackage[i]->Initialize();
	}
}

void delete_shape(int num) {
	HPEN delete_pen = CreatePen(PS_SOLID, shape[num]->pen_width, RGB(255, 255, 255));
	shape[num]->draw(delete_pen);	
}

void LoadAllShape(HWND hwnd) {
	int kind;
	for (int i = 0;i < shapenum;i++) {
		delete_shape(i);
		shape[i]->destroy();
	}
	FILE* fp = fopen(savePath, "r");
	if (fp == NULL) {

	}
	fscanf(fp, "%d", &shapenum); //读入图形数量
	for (int i = 0;i < shapenum;i++) {
		fscanf(fp, "%d", &kind);
		delete(builder);
		builder = shapePackage[kind-1]->GetBuilder();
		builder->build();
		builder->initialize(kind, hwnd);
		shape[i] = builder->returnShape();
		shape[i]->load(fp);
		shape[i]->kind = kind;
	}
	for (int i = 0;i < shapenum;i++) {
		shape[i]->draw(shape[i]->hPen);
	}
	fclose(fp);
}

void SaveAllShape() {
	FILE* fp = fopen(savePath, "w");
	if (fp == NULL) {
	}
	fprintf(fp, "%d \n", shapenum); //写入当前图形数量
	//fprintf(fp, "%d \n", kindNumber); //写入当前图形数量
	for (int i = 0;i < shapenum;i++) {
		fprintf(fp, "%d ", shape[i]->kind);
		shape[i]->save(fp);  //写入图形数据
	}
	fclose(fp);
}


//运动控制类

class ShapeMove
{
protected:
	int shapeNumber = 0;
	Point firstPosi;
	Point lastPosi;
public:
	bool moving = false;
	void LButtonDown(LPARAM lParam) {

		if (!moving) {

			while (shapeNumber < shapenum && !shape[shapeNumber]->click(lParam)) {
				shapeNumber++;
			}
			if (shapeNumber != shapenum) {   //在某个图形上，进入移动状态	
				moving = true;
				firstPosi.x = GET_X_LPARAM(lParam);
				firstPosi.y = GET_Y_LPARAM(lParam);
			}
			else           //空点击
				shapeNumber = 0;

		}
		else {
			moving = false;
			for (int i = 0;i < shapenum;i++) {       //防止图形移动导致该图形遮挡的别的图形缺失（清除函数），除移动图形外所有图形重画
				if (i != shapeNumber)
					shape[i]->draw(shape[i]->hPen);
			}
			shapeNumber = 0;
		}
	};
	void MouseMoving(LPARAM lParam) {
		if (moving) {
			lastPosi.x = GET_X_LPARAM(lParam);
			lastPosi.y = GET_Y_LPARAM(lParam);
			delete_shape(shapeNumber);
			shape[shapeNumber]->move(lastPosi.x - firstPosi.x, lastPosi.y - firstPosi.y);
			shape[shapeNumber]->draw(shape[shapeNumber]->hPen);
			firstPosi.x = lastPosi.x;
			firstPosi.y = lastPosi.y;
		}
	}
};
ShapeMove* moveController = new ShapeMove;






int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)

{
	//createShapePackage();
	InitShapePackage();

	static TCHAR szAppName[] = TEXT("MyWindows");
	HWND hwnd;  //窗口句柄，类似指针（可以看作窗口实例）
	MSG msg;    //消息是一个结构
	WNDCLASS wndclass;   //定义窗口类结构

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	//窗口类型     改变窗口宽，高时，动态改变窗口
	wndclass.lpfnWndProc = WndProc;  //指出对应的消息处理函数地址
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	//预留的额外空间，一般为0
	wndclass.hInstance = hInstance;
	//应用程序的实例句柄
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//窗口的图标
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//窗口的光标
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	//背景，白色
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	//窗口类名字

	if (!RegisterClass(&wndclass))   //注册窗口类且成功
	{
		MessageBox(NULL, TEXT("窗口类名字"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName,     //窗口实例化：wndclass->hwnd
		TEXT("画板"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hwnd, iCmdShow);     //显示窗口
	UpdateWindow(hwnd);            //更新窗口

	//函数调用


	while (GetMessage(&msg, NULL, 0, 0))    //持续接受消息
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg); //通过windows调用 callback wndproc
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	HDC hdc;
	HDC hdc1 = GetDC(hwnd);
	PAINTSTRUCT ps;
	RECT rect;
	static POINT Mouse_x_y;
	GetCursorPos(&Mouse_x_y);
	static  HWND  buttonDraw[10], save, load;//save_button, read_button;
	//static HWND b[5];
	switch (message)
	{
	case WM_CREATE:
		//绘制按钮
		
		for (int i = 0;i < kindnumber;i++) {
			buttonDraw[i] = CreateWindow(TEXT("Button"), shapePackage[i]->GetShapeName().c_str(),
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				30, 30 + i * 70, 200, 50,
				hwnd, (HMENU)1,
				((LPCREATESTRUCT)lParam)->hInstance, NULL);
		}
		load = CreateWindow(TEXT("Button"), TEXT("Load"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			30, 380, 200, 50,
			hwnd, (HMENU)1,
			((LPCREATESTRUCT)lParam)->hInstance, NULL);
		save = CreateWindow(TEXT("Button"), TEXT("Save"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			30, 450, 200, 50,
			hwnd, (HMENU)1,
			((LPCREATESTRUCT)lParam)->hInstance, NULL);

		return 0;

	case WM_COMMAND:    //按钮被按下后的响应

		for (int i = 0;i < kindnumber;i++) {
			if ((HWND)lParam == buttonDraw[i]) {				
				delete(builder);
				delete(painter);
				builder = shapePackage[i]->GetBuilder();
				builder->build();
				builder->initialize(i + 1, hwnd);
				shape[shapenum] = builder->returnShape();
				painter = shapePackage[i]->GetPainter();
				painter->GetShape(shape[shapenum]);
				painter->painting = true;
				break;
			}
		}

		if ((HWND)lParam == load) {
			if (painter)
				painter->painting = false;
			LoadAllShape(hwnd);
		}

		else if ((HWND)lParam == save) {
			if (painter)
				painter->painting = false;
			SaveAllShape();
		}
		return 0;

	case WM_PAINT:  //重绘窗口
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_LBUTTONDOWN:    //鼠标左键按下
		if (painter && painter->painting == true) {     //如果是按下了按钮后的绘制状态
			painter->LButtonDown(lParam);
			if (painter->painting == false) {
				shapenum++;
			}
		}
		else     //非绘制状态，判断是否拖动图形
		{
			moveController->LButtonDown(lParam);
		}
		return 0;

	case WM_MOUSEMOVE:
		if (painter && painter->painting == true) {
			painter->MouseMove(lParam);
		}
		else if (moveController && moveController->moving == true) {
			moveController->MouseMoving(lParam);
		}
		return 0;

	case WM_LBUTTONUP:
		return 0;

	case WM_LBUTTONDBLCLK:
		return 0;

	case WM_RBUTTONDOWN:
		if (painter && painter->painting == true) {
			painter->RButtonDown(lParam);
			if (painter->painting == false) {
				shapenum++;
			}
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
