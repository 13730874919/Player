//
// Created by unionman on 2020/4/1.
//

#include <XData.h>
#include "IDemux.h"
#include "XLog.h"


bool IDemux::Open(const  char *url){

}
XData IDemux::Read(){

}

void IDemux::Main() {
    while(!isExit){
        XData data = Read();
        if(data.size>0){
       //     XLOGE("IDemux read data addr size %d   %d",data.size,(long long)data.data);
            Notify(data);
        }

    //    XLOGI("IDemux Read pts==%d",data.pts);
    }
}
