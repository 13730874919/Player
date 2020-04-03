//
// Created by unionman on 2020/4/1.
//

#include "IDemux.h"
#include "XLog.h"

bool IDemux::Open(const  char *url){

}
XData IDemux::Read(){

}

void IDemux::Main() {
    while(!isExit){
        XData data = Read();
        XLOGI("IDemux Read %d",data.size);
    }
}
