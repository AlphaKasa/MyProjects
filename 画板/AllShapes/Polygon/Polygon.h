#pragma once

#ifndef POLYGON_H
#define POLYGON_H
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

class Poly :public Shape     //子类：多边形
{
public:
	Point point[10];  //绘制图形的点集	
	int num = 0;
	void draw(HPEN hp) {
		HDC  hdc;
		hdc = GetDC(hwnd);
		//SetROP2(hdc, R2_NOT);
		//SelectObject(hdc, GetStockObject(NULL_BRUSH));
		SelectObject(hdc, hp);
		MoveToEx(hdc, point[0].x, point[0].y, NULL);
		for (int i = 1;i < num;i++) {
			LineTo(hdc, point[i].x, point[i].y);
		}
		LineTo(hdc, point[0].x, point[0].y);

		ReleaseDC(hwnd, hdc);
	}
	void destroy() {
		delete(this);
	}
	void move(int x, int y) {
		for (int i = 0;i < num;i++) {
			point[i].x += x;
			point[i].y += y;
		}
	}
	virtual bool click(LPARAM lParam) {
		int x_max = point[0].x, x_min = point[0].x, y_max = point[0].y, y_min = point[0].y;
		for (int i = 0;i < num;i++) {
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
		fscanf(fp, "%d", &num);
		for (int i = 0;i < num;i++) {
			fscanf(fp, "%d", &(point[i].x));
			fscanf(fp, "%d", &(point[i].y));
		}

	}
	void save(FILE* fp) {
		fprintf(fp, "%d ", num);
		for (int j = 0;j < num;j++) {
			fprintf(fp, "%d ", point[j].x);
			fprintf(fp, "%d ", point[j].y);
		}
		fprintf(fp, "\n", NULL);
	}
};

class PolyBuilder :public Builder
{
	Poly* poly;

	void build() {
		poly = new Poly;
	}
	void  initialize(int k, HWND h) {
		poly->kind = k;
		poly->hwnd = h;
	}
	Shape* returnShape() {
		return poly;
	}
};


class PolyPainter :public Painter
{
public:
	Poly* poly;
	int pointnum = 0;
	void GetShape(Shape* s) {
		poly = (Poly*)s;
	}
	void LButtonDown(LPARAM lParam) {

		poly->point[pointnum].x = GET_X_LPARAM(lParam);
		poly->point[pointnum].y = GET_Y_LPARAM(lParam);
		pointnum++;

	}
	void LButtonDbClick(LPARAM lParam) {

	}
	void LButtonUp(LPARAM lParam) {

	}
	void RButtonDown(LPARAM lParam) {
		poly->point[pointnum].x = GET_X_LPARAM(lParam);
		poly->point[pointnum].y = GET_Y_LPARAM(lParam);
		pointnum++;
		poly->num = pointnum;

		pointnum = 0;
		poly->draw(poly->hPen);
		//shapenum++;
		painting = false;
	}
	void RButtonUp(LPARAM lParam) {

	}
	void MouseMove(LPARAM lParam) {

	}
};

class PolyPackage :public ShapePackage {
public:
	string GetShapeName() {
		return "Polygon";
	}
	Builder* GetBuilder() {
		return new PolyBuilder;
	}
	Painter* GetPainter() {
		return new PolyPainter;
	}
};

extern "C" _declspec(dllexport) ShapePackage * GetPackage() {
	return new PolyPackage;
}
#endif