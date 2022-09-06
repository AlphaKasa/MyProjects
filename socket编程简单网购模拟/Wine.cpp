#include<iostream>
#include<stdio.h>
#include<string>
#include<string.h>
#include"Commodity.h"
using namespace std;
class Wine: public Commodity{
public:
    void Initialization(){
        id=1;
        strcpy(name,"wine");
        price=10;
    }
};

extern "C" Commodity* GetCommodity()
{
	Wine* w=new Wine();
    w->Initialization();
	return w;
}
extern "C" int f()
{
	
	return 3;
}