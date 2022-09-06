#ifndef _SHOP1_H_
#define _SHOP1_H_
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
class Shop1 : public Shop{
public:
    bool CetPluginNames(){
        shop_id=1;

        DIR *dir = opendir("./alcohol-plugin");
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

	        string str = "./alcohol-plugin/";
	        str += pentry->d_name;
	        vstrPluginNames.push_back(str);
        }
        //cout<<"vstrPluginNames.size="<<vstrPluginNames.size()<<endl<<endl;
        closedir(dir);

        return true;
    }
} ;
#endif