#include <jni.h>
#include <string>
#include <android/log.h>
#include<iostream>
#include <IDecode.h>
#include <FFDecode.h>
#include "FFmpeg/FFDemux.h"
#include "IPlayer/IDemux.h"
#include "XThread.h"
#include "XLog.h"
#include <android/native_window_jni.h>
#include <XEGL.h>
#include <XShader.h>
#include <IVideoView.h>
#include <GLVideoView.h>
#include <IResample.h>
#include <FFResample.h>

extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavcodec/jni.h>
}

class testObds : public IObserver{
public:
    void Update(XData data) override {
        IObserver::Update(data);
     //   XLOGI("Observer::Update data.sizi =%d",data.size);
    }
};
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
IVideoView *view = NULL;
extern "C" JNIEXPORT jstring JNICALL
Java_com_bds_ffmpeg_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "6666";

    IDemux *de = new FFDemux();
    de->Open("sdcard/1080.mp4");

    IDecode *vdecode = new FFDecode();
    IDecode *adecode = new FFDecode();
    IResample *resample = new FFResample();
    view = new GLVideoView();

    vdecode->Open(de->GetVPara());
    adecode->Open(de->GetAPara());
    resample->init(de->GetAPara());
    de->AddObs(vdecode);
    de->AddObs(adecode);
    adecode->AddObs(resample);
    vdecode->AddObs(view);
    de->start();
    vdecode->start();
    adecode->start();




    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_bds_ffmpeg_XPlay_InitView(JNIEnv *env, jobject thiz, jobject obj) {
    // TODO: implement InitView()
    ANativeWindow *win = ANativeWindow_fromSurface(env,obj);
    view->setRender(win);

}