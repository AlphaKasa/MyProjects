#include<iostream>
#include<stdio.h>
#include<string>
#include<string.h>
#include"Commodity.h"
using namespace std;
class Beer: public Commodity{
public:
    void Initialization(){
        id=2;
        strcpy(name,"beer");
        price=4;
    }
};

extern "C" Commodity* GetCommodity()
{
	Beer* b=new Beer();
    b->Initialization();
	return b;
}
