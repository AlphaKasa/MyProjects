#ifndef _SHOP_H_
#define _SHOP_H_
#include<iostream>
#include<stdio.h>
#include<string>
#include<string.h>
#include<vector>

#include <dirent.h>
#include "Commodity.h"
#include "dlfcn.h"

class Shop{
public:
    int shop_id;
    vector<string> vstrPluginNames;
    vector<Commodity*> AllCommodity;
    float Sell(int id,int num){
        for(int i=0;i<AllCommodity.size();i++){
            if(AllCommodity[i]->id==id){               
                cout<<"您购买了"<<num<<"个"<<AllCommodity[i]->name<<",共花费"<<AllCommodity[i]->price*num<<"元!"<<endl;
                return AllCommodity[i]->price*num;
            }
        }
        return 0;
    }    
    void Show(){
        //cout<<"AllCommodity.size="<<AllCommodity.size()<<endl;
        cout<<"本店编号为:"<<shop_id<<",商品列表如下:"<<endl;
        for(int i = 0; i < AllCommodity.size(); i++){
            cout<<"商品号"<<AllCommodity[i]->id<<":"<<AllCommodity[i]->name<<" 价格:"<<AllCommodity[i]->price<<endl;
        }
        cout<<endl;
    }
    void InitializeCommodity(){
         //cout<<vstrPluginNames.size()<<endl;
        for(int i=0 ; i<vstrPluginNames.size(); i++)
        {
            //cout<<vstrPluginNames[i]<<endl;
            typedef Commodity* (*PLUGIN_CREATE)();
            //typedef int (*ff)();//
            PLUGIN_CREATE CreateProc; 
            //ff fff;

            Commodity *pPlugin = NULL;

            void* hinstLib = dlopen(vstrPluginNames[i].c_str(), RTLD_LAZY); 

            if(hinstLib != NULL) 
            { 
                //cout<<"hinstLib != NULL"<<endl;
                //m_vhForPlugin.push_back(hinstLib);

                CreateProc = (PLUGIN_CREATE)dlsym(hinstLib, "GetCommodity"); 
                //fff= (ff)dlsym(hinstLib, "f"); 
                if(NULL != CreateProc) 
                {
                    //cout<<"NULL != CreateProc"<<endl;
                    pPlugin=CreateProc();

                    if(pPlugin != NULL)
                    {
                        //cout<<"pPlugin != NULL"<<endl;
                        AllCommodity.push_back(pPlugin);
                    }
                }
                //if(fff!=NULL){
                    //cout<<fff()<<endl;
                //}
            }
        }   	
    }
      
    virtual bool CetPluginNames()=0;
        
    
};
#endif