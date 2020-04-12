//
// Created by bds on 2020/4/11.
//

#include <XLog.h>
#include "IPlayer.h"

IPlayer *IPlayer::Get(unsigned char index) {
    static IPlayer p[10];
    return &p[index];
}

bool IPlayer::Open(const char *path) {
    mux.lock();
    //解封装
    if(!demux || !demux->Open(path))
    {
        mux.unlock();
        XLOGE("demux->Open %s failed!",path);
        return false;
    }
    //解码 解码可能不需要，如果是解封之后就是原始数据
    if(!vdecode || !vdecode->Open(demux->GetVPara(),isHardDecode))
    {
        XLOGE("vdecode->Open %s failed!",path);
        //return false;
    }

    if(!adecode || !adecode->Open(demux->GetAPara()))
    {
        XLOGE("adecode->Open %s failed!",path);
        //return false;
    }

    //重采样 有可能不需要，解码后或者解封后可能是直接能播放的数据
    if(outPara.sample_rate <= 0)
        outPara = demux->GetAPara();
    if(!resample || !resample->init(demux->GetAPara(),outPara))
    {
        XLOGE("resample->Open %s failed!",path);
    }
    mux.unlock();
    return true;
}

bool IPlayer::Start() {
    mux.lock();
    if(!demux || !demux->start())
    {
        mux.unlock();
        XLOGE("demux->Start failed!");
        return false;
    }
    if(adecode)
        adecode->start();
    if(audioPlay)
        audioPlay->StartPlay(outPara);
    if(vdecode)
        vdecode->start();
    XThread::start();
    mux.unlock();
    return true;
}

void IPlayer::InitView(void *win) {
      if(videoView)
        videoView->setRender(win);
}

void IPlayer::Main() {
    while (!isExit)
    {
        mux.lock();
        if(!audioPlay|| !vdecode)
        {
            mux.unlock();
            XSleep(2);
            continue;
        }

        //同步
        //获取音频的pts 告诉视频
        int apts = audioPlay->pts;
        XLOGE("apts = %d",apts);
        vdecode->synPts = apts;

        mux.unlock();
        XSleep(2);
    }
}
