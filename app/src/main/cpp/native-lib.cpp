#include <jni.h>
#include <string>
#include <android/log.h>
#include<iostream>
#include "FFDemux.h"
#include "XThread.h"

#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,"testff",__VA_ARGS__)
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavcodec/jni.h>
}
extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm,void *res)
{
    av_jni_set_java_vm(vm,0);
    return JNI_VERSION_1_4;
}

long long GetNowMs(){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    int sec = tv.tv_sec%360000;
    int ret = sec*1000+tv.tv_usec/1000;
    return ret;
}

static double r2d(AVRational r){
    return r.num==0||r.den==0?0:(double)r.num/(double)r.den;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_bds_ffmpeg_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "6666";

    IDemux *de = new FFDemux();
    de->Open("sdcard/4k.mp4");
    de->start();
    XSleep(3000);
    de->stop();


//    hello += avcodec_configuration();
//    //初始化解封装
//    av_register_all();
//    //初始化网络
//    avformat_network_init();
//    avcodec_register_all();
//    AVFormatContext *ic = NULL;
//    char path[] = "/sdcard/1080.mp4";
//    int re = avformat_open_input(&ic,path,0,0);
//    if(re == 0)
//    {
//        LOGW("avformat_open_input %s success!",path);
//    }
//    else{
//        LOGW("avformat_open_input failed");
//        return env->NewStringUTF(hello.c_str());
//    }
//    re = avformat_find_stream_info(ic, 0);
//    if(re != 0){
//        LOGW("avformat_find_stream_info failed!");
//    }
//    LOGW("duratio = %lld, nb_stream = %d",ic->duration, ic->nb_streams);
//
//    int fps = 0;
//    int VideaoStream = 0;
//    int AudioStream = 1;
//
//    for (int i = 0; i <ic->nb_streams ; i++) {
//        AVStream *as = ic->streams[i];
//        if (as->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
//            LOGW("video");
//            fps = r2d(as->avg_frame_rate);
//            LOGW("fps = %d width = %d heignt = %d codecid = %d pixfomat = %d",fps,
//                    as->codecpar->width,
//                    as->codecpar->height,
//                    as->codecpar->codec_id,
//                    as->codecpar->format);
//            VideaoStream = i;
//        } else if (as->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
//            LOGW("audio");
//            AudioStream = i;
//            LOGW("sample_rate = %d chnnel = %d format = %d",
//                 as->codecpar->sample_rate,
//                 as->codecpar->channels,
//                 as->codecpar->format);
//        }
//    }
//    AudioStream =av_find_best_stream(ic,AVMEDIA_TYPE_AUDIO,-1,-1,NULL,0);
//    LOGW("av_find_best_stream audioStream = %d",AudioStream);
//
//    //打开解码器
//    //软解码
//    AVCodec *codec = avcodec_find_decoder(ic->streams[VideaoStream]->codecpar->codec_id);
//    //硬解码
//  //  codec = avcodec_find_decoder_by_name("h264_mediacodec");
////    if(ic->streams[VideaoStream]->codecpar->codec_id == AV_CODEC_ID_H264){
////        LOGW("H264 codec");
////        codec = avcodec_find_decoder_by_name("h264_mediacodec");
////    }
////    else if(ic->streams[VideaoStream]->codecpar->codec_id == AV_CODEC_ID_MPEG4){
////        codec = avcodec_find_decoder_by_name("mpeg4_mediacodec");
////        LOGW("mpeg4 codec");
////    }
//    if(!codec){
//        LOGW("get codec failed");
//        return env->NewStringUTF(hello.c_str());
//    }
//    //解码器初始化
//    AVCodecContext *vc = avcodec_alloc_context3(codec);
//    avcodec_parameters_to_context(vc,ic->streams[VideaoStream]->codecpar);
//    vc->thread_count = 1;
//
//    //打开视频解码器
//    re = avcodec_open2(vc,codec,0);
//    LOGW("vc timebase = %d/ %d  codec_id==%d",vc->time_base.num,vc->time_base.den,ic->streams[AudioStream]->codecpar->codec_id);
//    if(re != 0)
//    {
//        LOGW("avcodec_open2 video failed!");
//        return env->NewStringUTF(hello.c_str());
//    }
//
//
//    /////////////////////////////////////
//    //打开音频解码器
//    AVCodec *acodec = avcodec_find_decoder(ic->streams[AudioStream]->codecpar->codec_id);
//    //硬解码
//    //  acodec = avcodec_find_decoder_by_name("h264_mediacodec");
//    if(!acodec)
//    {
//        LOGW("avcodec_find failed!");
//        return env->NewStringUTF(hello.c_str());
//    }
//    //初始化解码器
//    AVCodecContext *ac = avcodec_alloc_context3(acodec);
//    avcodec_parameters_to_context(ac,ic->streams[AudioStream]->codecpar);
//    ac->thread_count = 1;
//    //打开解码器
//    re = avcodec_open2(ac, acodec,0);
//    if(re != 0)
//    {
//        LOGW("avcodec_open2  audio failed!");
//        return env->NewStringUTF(hello.c_str());
//    }
//    //获取帧数据
//    AVPacket *pkt = av_packet_alloc();
//    AVFrame *frame = av_frame_alloc();
//    long long start = GetNowMs();
//    int frame_cnt = 0;
//    while (true){
//        if(GetNowMs()-start>=3000){
//            int fps = frame_cnt/3;
//            start = GetNowMs();
//            frame_cnt  = 0;
//            LOGW("now decode fps is %d",fps);
//        }
//        re = av_read_frame(ic,pkt);
//        if(re!=0){
//            LOGW("文件结尾");
//            int pos = 20*r2d(ic->streams[VideaoStream]->time_base);
//            av_seek_frame(ic,VideaoStream,pos,AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_FRAME);
//            continue;
//        }
//        AVCodecContext *cc = vc;
//        if(pkt->stream_index == VideaoStream) cc = vc;
//        else if(pkt->stream_index == AudioStream)cc = ac;
//    //    LOGW("stream = %d size =%d pts=%lld flag=%d",pkt->stream_index,pkt->size,pkt->pts,pkt->dts);
//        //发送到线程中解码
//        re = avcodec_send_packet(cc,pkt);
//        av_packet_unref(pkt);
//        if(re != 0)
//        {
//            LOGW("avcodec_send_packet failed!");
//            continue;
//        }
//        for(;;){
//            re = avcodec_receive_frame(cc,frame);
//            if(re != 0){
//                break;
//            }
//            if(cc == vc){
//                frame_cnt++;
//            }
//        }
//        av_packet_unref(pkt);
//    }
    //关闭上下文
//    avformat_close_input(&ic);
    return env->NewStringUTF(hello.c_str());
}
