#pragma once

#ifndef RECTANGLE_H
#define RECTANGLE_H
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

class Rect :public Shape       //子类：矩形
{
public:
	Point point[2];  //绘制图形的点集

	void draw(HPEN hp) {
		HDC  hdc;
		hdc = GetDC(hwnd);
		SelectObject(hdc, hp);
		Rectangle(hdc, point[0].x, point[0].y, point[1].x, point[1].y);
		ReleaseDC(hwnd, hdc);
	}
	void destroy() {
		delete(this);
	}
	void move(int x, int y) {
		for (int i = 0;i < 2;i++) {
			point[i].x += x;
			point[i].y += y;
		}
	}
	virtual bool click(LPARAM lParam) {
		if (point[0].x < GET_X_LPARAM(lParam) && GET_X_LPARAM(lParam) < point[1].x || point[1].x < GET_X_LPARAM(lParam) && GET_X_LPARAM(lParam) < point[0].x) {
			if (point[0].y < GET_Y_LPARAM(lParam) && GET_Y_LPARAM(lParam) < point[1].y || point[1].y < GET_Y_LPARAM(lParam) && GET_Y_LPARAM(lParam) < point[0].y) {
				return true;
			}
		}
		return false;
	}
	void load(FILE* fp) {
		fscanf(fp, "%d", &(point[0].x));
		fscanf(fp, "%d", &(point[0].y));
		fscanf(fp, "%d", &(point[1].x));
		fscanf(fp, "%d", &(point[1].y));
	}
	void save(FILE* fp) {
		for (int j = 0;j < 2;j++) {
			fprintf(fp, "%d ", point[j].x);
			fprintf(fp, "%d ", point[j].y);
		}
		fprintf(fp, "\n", NULL);
	}
};


class RectBuilder :public Builder
{
	Rect* rect;
	void build() {
		rect = new Rect;
	}
	void  initialize(int k, HWND h) {
		rect->kind = k;
		rect->hwnd = h;
	}
	Shape* returnShape() {
		return rect;
	}
};

class RectPainter :public Painter
{
public:
	Rect* rect;
	int pointnum = 0;
	void GetShape(Shape* s) {
		rect = (Rect*)s;
	}
	void LButtonDown(LPARAM lParam) {
		pointnum++;
		if (pointnum < 2) {
			for (int i = 0;i < 2;i++) {
				rect->point[i].x = GET_X_LPARAM(lParam);
				rect->point[i].y = GET_Y_LPARAM(lParam);
			}
		}
		else {
			rect->point[1].x = GET_X_LPARAM(lParam);
			rect->point[1].y = GET_Y_LPARAM(lParam);
			pointnum = 0;
			rect->draw(rect->hPen);

			//shapenum++;
			painting = false;
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
		if (pointnum == 1) {
			rect->draw(CreatePen(PS_SOLID, rect->pen_width, RGB(255, 255, 255)));
			rect->point[1].x = GET_X_LPARAM(lParam);
			rect->point[1].y = GET_Y_LPARAM(lParam);
			rect->draw(rect->hPen);
		}
	}
};

class RectPackage :public ShapePackage {
public:
	string GetShapeName() {
		return "Rectangle";
	}
	Builder* GetBuilder() {
		return new RectBuilder;
	}
	Painter* GetPainter() {
		return new RectPainter;
	}
};

extern "C" _declspec(dllexport) ShapePackage * GetPackage() {
	return new RectPackage;
}

#endif