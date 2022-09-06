#ifndef _SHOP2_H_
#define _SHOP2_H_
#include<iostream>
#include<stdio.h>
#include<string>
#include<string.h>
#include<vector>

#include <dirent.h>
//#include "Commodity.h"
#include "dlfcn.h"
#include "Shop.h"
using namespace std;
class Shop2 : public Shop{
public:
    bool CetPluginNames(){
        shop_id=2;

        DIR *dir = opendir("./cosmetic-plugin");
        if(dir == 0){
            cout<<"no dir"<<endl;
	        return false;
        }
        for(;;)
        {
	        struct dirent *pentry = readdir(dir);
	        if(pentry == 0)
	            break;

	        if(strcmp(pentry->d_name, ".") == 0)
	            continue;

	        if(strcmp(pentry->d_name, "..") == 0)
	            continue;

	        string str = "./cosmetic-plugin/";
	        str += pentry->d_name;
	        vstrPluginNames.push_back(str);
        }      
        closedir(dir);

        return true;
    }
} ;
#endif