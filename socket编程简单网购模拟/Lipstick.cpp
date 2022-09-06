#include<iostream>
#include<stdio.h>
#include<string>
#include<string.h>
#include"Commodity.h"
using namespace std;
class Lipstick: public Commodity{
public:
    void Initialization(){
        id=3;
        strcpy(name,"lipstick");
        price=45;
    }
};

extern "C" Commodity* GetCommodity()
{
	Lipstick* l=new Lipstick();
    l->Initialization();
	return l;
}