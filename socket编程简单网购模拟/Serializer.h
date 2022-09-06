#ifndef _SERIALIZER_H_
#define _SERIALIZER_H_
#include<iostream>
#include<stdio.h>
#include<string>
#include<string.h>
#include<vector>

#include <dirent.h>
#include "Commodity.h"
#include "dlfcn.h"
#endif

class Serializer{
public:
    Commodity* commodity[100];
    void GetAllCommodity1(){
        vector<string> vstrPluginNames;
    
        DIR *dir = opendir("./alcohol-plugin");
            if(dir == 0){
                cout<<"no dir"<<endl;	        
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
            for(int i=0 ; i<vstrPluginNames.size(); i++)
            {
            
                typedef Commodity* (*PLUGIN_CREATE)();        
                PLUGIN_CREATE CreateProc;    
                Commodity *pPlugin = NULL;

                void* hinstLib = dlopen(vstrPluginNames[i].c_str(), RTLD_LAZY); 

                if(hinstLib != NULL) 
                { 
                    
                    CreateProc = (PLUGIN_CREATE)dlsym(hinstLib, "GetCommodity"); 
                
                    if(NULL != CreateProc) 
                    {                 
                        pPlugin=CreateProc();

                        if(pPlugin != NULL)
                        {                    
                            commodity[pPlugin->id]=pPlugin;
                        }
                    }
                
                }
            }
    }
    void GetAllCommodity2(){
        vector<string> vstrPluginNames;
    
        DIR *dir = opendir("./cosmetic-plugin");
            if(dir == 0){
                cout<<"no dir"<<endl;	        
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
            for(int i=0 ; i<vstrPluginNames.size(); i++)
            {
            
                typedef Commodity* (*PLUGIN_CREATE)();        
                PLUGIN_CREATE CreateProc;    
                Commodity *pPlugin = NULL;

                void* hinstLib = dlopen(vstrPluginNames[i].c_str(), RTLD_LAZY); 

                if(hinstLib != NULL) 
                { 
                    
                    CreateProc = (PLUGIN_CREATE)dlsym(hinstLib, "GetCommodity"); 
                
                    if(NULL != CreateProc) 
                    {                 
                        pPlugin=CreateProc();

                        if(pPlugin != NULL)
                        {                    
                            commodity[pPlugin->id]=pPlugin;
                        }
                    }
                
                }
            }
    }
    void Initialization(){
        GetAllCommodity1();
        GetAllCommodity2();
    }
    void Trans(int i,vector<Commodity*> &v){  //i为1：将服务器2商品转移至服务器1；i为1：将服务器1商品转移至服务器2
        //实际操作为将
        FILE *fp ;      
       
        if(i==1){
            fp=fopen("data2", "r");
        }
        else if(i==2){
            fp=fopen("data1", "r");
        }
      
        int num;//商品数目
        fread(&num, sizeof(int), 1, fp);
        int kind=0;
        for(int i=0;i<num;i++){
            fread(&kind, sizeof(int), 1, fp);
            v.push_back(commodity[kind]);
        }
        //id,name,price
        
        fclose(fp);
    }

    Commodity* f2(int i){
        return commodity[i];
    }
};