#ifndef _COMMODITY_H_
#define _COMMODITY_H_
#include<iostream>
#include<stdio.h>
#include<string>
using namespace std;
class Commodity{
public:
    int id; 
    char name[20];
    float price;
    virtual void Initialization()=0;
};
#endif