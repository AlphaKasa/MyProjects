#pragma once

#ifndef TRIANGLE_H
#define TRIANGLE_H
#include<unordered_map>
#include<windows.h>
#include<windowsx.h>
#include<math.h>
#include<stdio.h>
#include <string>
#include <string.h>
#include<typeinfo>

#include "F:/工作/画板/Shape/Shape.h"
#pragma comment(lib,"F:/工作/画板/Shape/Debug/SHAPE.lib")

using namespace std;

class Triangle :public Shape     //子类：三角形（多边形）
{
public:
	Point point[3];  //绘制图形的点集	

	void draw(HPEN hp) {
		HDC  hdc;
		hdc = GetDC(hwnd);
		//SetROP2(hdc, R2_NOT);
		//SelectObject(hdc, GetStockObject(NULL_BRUSH));
		SelectObject(hdc, hp);
		MoveToEx(hdc, point[0].x, point[0].y, NULL);
		for (int i = 1;i < 3;i++) {
			LineTo(hdc, point[i].x, point[i].y);
		}
		LineTo(hdc, point[0].x, point[0].y);

		ReleaseDC(hwnd, hdc);
	}
	void destroy() {
		delete(this);
	}
	void move(int x, int y) {
		for (int i = 0;i < 3;i++) {
			point[i].x += x;
			point[i].y += y;
		}
	}
	virtual bool click(LPARAM lParam) {
		int x_max = point[0].x, x_min = point[0].x, y_max = point[0].y, y_min = point[0].y;
		for (int i = 0;i < 3;i++) {
			if (x_max < point[i].x) {
				x_max = point[i].x;
			}
			if (x_min > point[i].x) {
				x_min = point[i].x;
			}
			if (y_max < point[i].y) {
				y_max = point[i].y;
			}
			if (y_min > point[i].y) {
				y_min = point[i].y;
			}
		}
		if (y_min < GET_Y_LPARAM(lParam) && GET_Y_LPARAM(lParam) < y_max && x_min < GET_X_LPARAM(lParam) && GET_X_LPARAM(lParam) < x_max) {
			return true;
		}
		return false;
	}
	void load(FILE* fp) {
		for (int i = 0;i < 3;i++) {
			fscanf(fp, "%d", &(point[i].x));
			fscanf(fp, "%d", &(point[i].y));
		}

	}
	void save(FILE* fp) {
		for (int j = 0;j < 3;j++) {
			fprintf(fp, "%d ", point[j].x);
			fprintf(fp, "%d ", point[j].y);
		}
		fprintf(fp, "\n", NULL);
	}
};

class TriaBuilder :public Builder
{
	Triangle* tria;
	void build() {
		tria = new Triangle;
	}
	void  initialize(int k, HWND h) {
		tria->kind = k;
		tria->hwnd = h;
	}
	Shape* returnShape() {
		return tria;
	}
};

class TriaPainter :public Painter
{
public:
	Triangle* tria;
	int pointnum = 0;
	void GetShape(Shape* s) {
		tria = (Triangle*)s;
	}
	void LButtonDown(LPARAM lParam) {
		if (pointnum < 3) {
			tria->point[pointnum].x = GET_X_LPARAM(lParam);
			tria->point[pointnum].y = GET_Y_LPARAM(lParam);
			pointnum++;
			if (pointnum == 3) {
				pointnum = 0;
				tria->draw(tria->hPen);

				//shapenum++;
				painting = false;
			}
		}
	}
	void LButtonDbClick(LPARAM lParam) {

	}
	void LButtonUp(LPARAM lParam) {

	}
	void RButtonDown(LPARAM lParam) {

	}
	void RButtonUp(LPARAM lParam) {

	}
	void MouseMove(LPARAM lParam) {

	}
};

class TriaPackage :public ShapePackage {
public:

	string GetShapeName() {
		return "Triangle";
	}
	Builder* GetBuilder() {
		return new TriaBuilder;
	}
	Painter* GetPainter() {
		return new TriaPainter;
	}
};

extern "C" _declspec(dllexport) ShapePackage * GetPackage() {
	return new TriaPackage;
}

#endif