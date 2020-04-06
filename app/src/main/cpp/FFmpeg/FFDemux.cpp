//
// Created by unionman on 2020/4/1.
//
extern "C"{
#include <libavformat/avformat.h>
}

#include <XData.h>
#include "FFDemux.h"
#include "XLog.h"



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
    GetVPara();
    GetAPara();
    return true;
}

XData FFDemux::Read() {
    if(!ic)return XData();
    XData data;
    AVPacket *pkt = av_packet_alloc();
    int re = av_read_frame(ic,pkt);
    if(re!=0){
       // XLOGE("av_read_frame read failed %d",re);
        av_packet_free(&pkt);
        return XData();
    }
  //  XLOGI(" FFDemux  pkt.siz=%d data", pkt->size);
    data.data = (unsigned char *)pkt;
    data.size = pkt->size;
    if(pkt->stream_index == audioStream)
    {
        data.isAudio = true;
    }
    else if(pkt->stream_index == videoStream)
    {
        data.isAudio = false;
    }
    else
    {
        av_packet_free(&pkt);
        return XData();
    }
   data.pts = pkt->pts;

  //  XLOGI("pk->size===%d  total===%d  pts==%d",pkt->size,total,pkt->pts);
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

XParameter FFDemux::GetVPara() {
    if (!ic) {
        XLOGE("GetVPara failed! ic is NULL！");
        return XParameter();
    }
    //获取了视频流索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0);
    if (re < 0) {
        XLOGE("av_find_best_stream failed!");
        return XParameter();
    }
    videoStream = re;
    XParameter para;
    para.para = ic->streams[re]->codecpar;


    return para;
}

XParameter FFDemux::GetAPara() {
    if (!ic) {
        XLOGE("GetVPara failed! ic is NULL！");
        return XParameter();
    }
    //获取了音频流索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, 0, 0);
    if (re < 0) {
        XLOGE("av_find_best_stream failed!");
        return XParameter();
    }
    audioStream = re;
    XParameter para;
    para.para = ic->streams[re]->codecpar;
    return para;
}


