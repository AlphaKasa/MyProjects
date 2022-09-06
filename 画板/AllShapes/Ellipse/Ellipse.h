#pragma once

#ifndef ELLIPSE_H
#define ELLIPSE_H
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

class Elli :public Shape     //子类：圆和椭圆
{
public:
	Point point;  //圆心
	int r;

	void draw(HPEN hp) {
		HDC  hdc;
		hdc = GetDC(hwnd);
		//SetROP2(hdc, R2_NOT);
		//SelectObject(hdc, GetStockObject(NULL_BRUSH));
		SelectObject(hdc, hp);
		Ellipse(hdc, point.x - r, point.y - r, point.x + r, point.y + r);

		ReleaseDC(hwnd, hdc);
	}
	void destroy() {
		delete(this);
	}
	void move(int x, int y) {
		point.x += x;
		point.y += y;
	}
	virtual bool click(LPARAM lParam) {
		if (point.x - r < GET_X_LPARAM(lParam) && GET_X_LPARAM(lParam) < point.x + r) {
			if (point.y - r < GET_Y_LPARAM(lParam) && GET_Y_LPARAM(lParam) < point.y + r) {
				return true;
			}
		}
		return false;
	}
	void load(FILE* fp) {
		fscanf(fp, "%d", &(point.x));
		fscanf(fp, "%d", &(point.y));
		fscanf(fp, "%d", &r);
	}
	void save(FILE* fp) {
		fprintf(fp, "%d ", point.x);
		fprintf(fp, "%d ", point.y);
		fprintf(fp, "%d ", r);

		fprintf(fp, "\n", NULL);
	}
};

class ElliBuilder :public Builder {
	Elli* elli;
	void build() {
		elli = new Elli;
	}
	void  initialize(int k, HWND h) {
		elli->kind = k;
		elli->hwnd = h;
	}
	Shape* returnShape() {
		return elli;
	}
};


class ElliPainter :public Painter {
public:
	Elli* elli;
	int pointnum = 0;
	void GetShape(Shape* s) {
		elli = (Elli*)s;
	}
	void LButtonDown(LPARAM lParam) {
		pointnum++;
		if (pointnum == 1) {
			elli->point.x = GET_X_LPARAM(lParam);
			elli->point.y = GET_Y_LPARAM(lParam);
		}
		else {
			elli->r = sqrt(pow(GET_X_LPARAM(lParam) - elli->point.x, 2) + pow(GET_Y_LPARAM(lParam) - elli->point.y, 2));
			pointnum = 0;
			elli->draw(elli->hPen);

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
			elli->draw(CreatePen(PS_SOLID, elli->pen_width, RGB(255, 255, 255)));
			elli->r = sqrt(pow(GET_X_LPARAM(lParam) - elli->point.x, 2) + pow(GET_Y_LPARAM(lParam) - elli->point.y, 2));
			elli->draw(elli->hPen);
		}
	}
};

class ElliPackage :public ShapePackage {
public:
	string GetShapeName() {
		return "Ellipse";
	}
	Builder* GetBuilder() {
		return new ElliBuilder;
	}
	Painter* GetPainter() {
		return new ElliPainter;
	}
};

extern "C" _declspec(dllexport) ShapePackage * GetPackage() {
	return new ElliPackage;
}

#endif