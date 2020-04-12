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
static double r2d(AVRational r)
{
    return r.num == 0 || r.den == 0 ?0.:(double) r.num/(double)r.den;
}
static int cnt =0;
XData FFDemux::Read() {
    if(!ic)return XData();
    XData data;
    AVPacket *pkt = av_packet_alloc();
    int re = av_read_frame(ic,pkt);
    if(re!=0){
        XLOGE("av_read_frame read failed %d",re);
        av_packet_free(&pkt);
        return XData();
    }
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
        //转换pts
        pkt->pts = pkt->pts * (1000*r2d(ic->streams[pkt->stream_index]->time_base));
        pkt->dts = pkt->dts * (1000*r2d(ic->streams[pkt->stream_index]->time_base));
        data.pts = (int)pkt->pts;
    return data;
}

FFDemux::FFDemux() {
    static bool isFirst = true;
    if(isFirst){
        isFirst = false;
        XLOGE("av_register_all ");
    //注册封装器
        av_register_all();

        avformat_network_init();
        //注册解码器
        avcodec_register_all();
        //初始化网络

    }
}

XParameter FFDemux::GetVPara() {
    if (!ic) {
        XLOGE("GetVPara failed! ic is NULL！");
        return XParameter();
    }
    //获取了视频流索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
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
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (re < 0) {
        XLOGE("av_find_best_stream failed!");
        return XParameter();
    }
    audioStream = re;
    XParameter para;\
    para.channels= ic->streams[re]->codecpar->channels;
    para.sample_rate = ic->streams[re]->codecpar->sample_rate;
    para.para = ic->streams[re]->codecpar;
    return para;
}


