#include <jni.h>
#include <string>
#include <android/log.h>
#include<iostream>
#include "FFmpeg/FFDemux.h"
#include "IPlayer/IDemux.h"
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
    de->Open("sdcard/1080.mp4");
    de->start();
    XSleep(3000);
    de->stop();


    return env->NewStringUTF(hello.c_str());
}
