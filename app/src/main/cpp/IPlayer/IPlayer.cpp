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
    Close();
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

    outPara = demux->GetAPara();
    if(!resample || !resample->init(demux->GetAPara(),outPara))
    {
        XLOGE("resample->Open %s failed!",path);
    }
    playStatus=PREPARE;
    mux.unlock();
    return true;
}

bool IPlayer::Start() {
    mux.lock();
    if(vdecode)
        vdecode->start();

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

    XThread::start();
    playStatus=PLAYING;
    mux.unlock();
    return true;
}

void IPlayer::InitView(void *win) {
      if(videoView){
          videoView->Close();
          videoView->setRender(win);
      }

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
      //  XLOGE("apts = %d",apts);
        vdecode->synPts = apts;

        mux.unlock();
        XSleep(2);
    }
}

void IPlayer::Close() {
    mux.lock();
    //2 先关闭主体线程，再清理观察者
    //同步线程
    XThread::stop();
    //解封装
    if(demux)
        demux->stop();
    //解码
    if(vdecode)
        vdecode->stop();
    if(adecode)
        adecode->stop();
    if(audioPlay)
        audioPlay->stop();
    //2 清理缓冲队列
    if(vdecode)
        vdecode->Clear();
    if(adecode)
        adecode->Clear();
    if(audioPlay)
        audioPlay->Clear();

    //3 清理资源
    if(audioPlay)
        audioPlay->Close();
    if(videoView)
        videoView->Close();
    if(vdecode)
        vdecode->Close();
    if(adecode)
        adecode->Close();
    if(demux)
        demux->Close();
    playStatus=IDEL;
    mux.unlock();
}

double IPlayer::PlayPos() {
    double pos = 0.0;
    mux.lock();

    int total = 0;
    if(demux)
        total = demux->total;
    if(total>0)
    {
        if(vdecode)
        {
            pos = (double)vdecode->pts/(double)total;
        }
    }
    mux.unlock();
    return pos;
}

bool IPlayer::Seek(double pos) {
    bool re = false;
    if(!demux) return false;

    //暂停所有线程
    SetPause(true);
    mux.lock();
    //清理缓冲
    //2 清理缓冲队列
    if(vdecode)
        vdecode->Clear(); //清理缓冲队列，清理ffmpeg的缓冲
    if(adecode)
        adecode->Clear();
    if(audioPlay)
        audioPlay->Clear();


    re = demux->Seek(pos); //seek跳转到关键帧
    if(!vdecode)
    {
        mux.unlock();
        SetPause(false);
        return re;
    }
//    //解码到实际需要显示的帧
//    long long seekPts = pos*demux->total;
//    XLOGE("seekpts==%lld",seekPts);
//    while(!isExit)
//    {
//        XData pkt = demux->Read();
//        if(pkt.size<=0)break;
//        if(pkt.isAudio)
//        {
//            if(pkt.pts < seekPts)
//            {
//                pkt.Drop();
//                continue;
//            }
//            //写入缓冲队列
//            demux->Notify(pkt);
//            continue;
//        }
//
//        //解码需要显示的帧之前的数据
//        vdecode->SendPacket(pkt);
//        pkt.Drop();
//        XData data = vdecode->RecvFrame();
//        if(data.size <=0)
//        {
//            continue;
//        }
//        XLOGE("data.pts==%d",data.pts);
//        if(data.pts >= seekPts)
//        {
//            //vdecode->Notify(data);
//            break;
//        }
//    }
    mux.unlock();

    SetPause(false);
    return re;
}

void IPlayer::SetPause(bool isP) {
    mux.lock();
    XThread::SetPause(isP);
    if(demux)
        demux->SetPause(isP);
    if(vdecode)
        vdecode->SetPause(isP);
    if(adecode)
        adecode->SetPause(isP);
    if(audioPlay)
        audioPlay->SetPause(isP);
    if(isP)
        playStatus=IDEL;
    else
        playStatus=PLAYING;
    mux.unlock();
}

int IPlayer::getDuration() {
    mux.lock();
    int total = 0;
    if(demux)
        total = demux->total;
    mux.unlock();
    return total;
}

int IPlayer::getPlayStatus() {
    return playStatus ;
}

int IPlayer::getVideoWidth() {
    mux.lock();
    int ret = 0;
    if(demux)
        ret = demux->VideoWidth;
    mux.unlock();
    return ret;
}

int IPlayer::getVideoHeight() {
    mux.lock();
    int ret = 0;
    if(demux)
        ret = demux->VideoHeight;
    mux.unlock();
    return ret;
}
