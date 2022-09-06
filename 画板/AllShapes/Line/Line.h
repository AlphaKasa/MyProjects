#pragma once

#ifndef LINE_H
#define LINE_H
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



class Line :public Shape   //子类：线段
{

public:
	Point point[2];  //绘制图形的点集

	void draw(HPEN hp) {
		HDC  hdc;
		hdc = GetDC(this->hwnd);
		SelectObject(hdc, hp);
		MoveToEx(hdc, point[0].x, point[0].y, NULL);
		LineTo(hdc, point[1].x, point[1].y);
		ReleaseDC(this->hwnd, hdc);
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

class LineBuilder :public Builder
{
	Line* line;
	void build() {
		line = new Line;
	}
	void  initialize(int k, HWND h) {
		line->kind = k;
		line->hwnd = h;
	}
	Shape* returnShape() {
		return line;
	}
};


class LinePainter :public Painter
{
public:
	Line* line;
	int pointnum = 0;
	void GetShape(Shape* s) {
		line = (Line*)s;
	}
	void LButtonDown(LPARAM lParam) {
		pointnum++;
		if (pointnum < 2) {
			for (int i = 0;i < 2;i++) {
				line->point[i].x = GET_X_LPARAM(lParam);
				line->point[i].y = GET_Y_LPARAM(lParam);
			}
		}
		else {
			pointnum = 0;
			line->point[1].x = GET_X_LPARAM(lParam);
			line->point[1].y = GET_Y_LPARAM(lParam);
			line->draw(line->hPen);			
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
			line->draw(CreatePen(PS_SOLID, line->pen_width, RGB(255, 255, 255)));
			line->point[1].x = GET_X_LPARAM(lParam);
			line->point[1].y = GET_Y_LPARAM(lParam);
			line->draw(line->hPen);
			x = 3;
		}
	}
};

class LinePackage :public ShapePackage {
public:
	string GetShapeName() {
		return "Line";
	}
	Builder* GetBuilder() {
		return new LineBuilder;
	}
	Painter* GetPainter() {
		return new LinePainter;
	}
};

extern "C" _declspec(dllexport) ShapePackage* GetPackage() {
	return new LinePackage;
}

#endif