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
#include <SLAudioPlay.h>
#include <IPlayer.h>
#include <IPlayerPorxy.h>

extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavcodec/jni.h>
}
IVideoView *view = NULL;
extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm,void *res)
{
    IPlayerPorxy::Get()->Init(vm);
    return JNI_VERSION_1_4;
}




extern "C"
JNIEXPORT void JNICALL
Java_com_bds_ffmpeg_XPlay_InitView(JNIEnv *env, jobject thiz, jobject obj) {
    // TODO: implement InitView()
    ANativeWindow *win = ANativeWindow_fromSurface(env,obj);
   // view->setRender(win);
    IPlayerPorxy::Get()->InitView(win);
}extern "C"
JNIEXPORT void JNICALL
Java_com_bds_ffmpeg_OpenUrl_Open(JNIEnv *env, jobject thiz, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);
    IPlayerPorxy::Get()->Open(url);
    IPlayerPorxy::Get()->Start();
    env->ReleaseStringUTFChars(url_, url);
}