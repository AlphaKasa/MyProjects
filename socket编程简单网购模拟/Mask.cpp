#include<iostream>
#include<stdio.h>
#include<string>
#include<string.h>
#include"Commodity.h"
using namespace std;
class Mask: public Commodity{
public:
    void Initialization(){
        id=4;
        strcpy(name,"mask");
        price=20;
    }
};

extern "C" Commodity* GetCommodity()
{
	Mask* m=new Mask();
    m->Initialization();
	return m;
}