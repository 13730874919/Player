//
// Created by unionman on 2020/4/1.
//
extern "C"{
#include <libavformat/avformat.h>
}

#include <../XData.h>
#include "FFDemux.h"
#include "../XLog.h"



bool FFDemux::Open(const  char *url) {
    XLOGI("open url %s",url);
    int ret = avformat_open_input(&ic,url,0,0);
    if(ret !=0){
        XLOGE("open failed %s",url);
        return false;
    }
    XLOGE("avformat_open_input success");
    ret = avformat_find_stream_info(ic,0);
    if(ret !=0){
        XLOGE("avformat_find_stream_info failed");
        return false;
    }
    XLOGE("avformat_find_stream_info success");
    this->total = ic->duration/(AV_TIME_BASE/1000);
    XLOGI("this->total= %d",total);
    return false;
}

XData FFDemux::Read() {
    if(!ic)return XData();
    XData data;
    AVPacket *pk = av_packet_alloc();
    int re = av_read_frame(ic,pk);
    if(re!=0){
        XLOGE("av_read_frame read failed %d",re);
        av_packet_free(&pk);
        return XData();
    }
    data.data = (unsigned char *)pk;
    data.size = pk->size;
    XLOGI("pk->size===%d  total===%d  pts==%d",pk->size,total,pk->pts);
    return data;
}

FFDemux::FFDemux() {
    static bool isFirst = true;
    if(isFirst){
        isFirst = false;
    //注册封装器
        av_register_all();
        //注册解码器
        avcodec_register_all();
        //初始化网络

        avformat_network_init();
    }
}


