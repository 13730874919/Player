//
// Created by unionman on 2020/4/1.
//

#include <XData.h>
#include "IDemux.h"
#include "XLog.h"



void IDemux::Main() {
    while(!isExit){
        if(IsPause())
        {
            XSleep(2);
            continue;
        }
        XData data = Read();
        if(data.size>0){
       //     XLOGE("IDemux read data addr size %d   %d",data.size,(long long)data.data);
            Notify(data);
        }else{  XSleep(2);}

    //    XLOGI("IDemux Read pts==%d",data.pts);
    }
}
