//
// Created by bds on 2020/4/11.
//

#ifndef FFMPEG_IPLAYER_H
#define FFMPEG_IPLAYER_H


#include "IDemux.h"
#include "IDecode.h"
#include "IResample.h"
#include "IVideoView.h"
#include "IAudioPlay.h"

class IPlayer : public IObserver{
public:
    static IPlayer *Get(unsigned char index=0);
    virtual bool Open(const char *path);
    virtual bool Start();
    virtual void InitView(void *win);
    virtual void Close();
    virtual double PlayPos();
    //是否视频硬解码
    bool isHardDecode = true;
    virtual bool Seek(double pos);
    virtual void SetPause(bool isP);

protected:
    void Main() override;

public:
    IDemux *demux = 0;
    IDecode *vdecode = 0;
    IDecode *adecode = 0;
    IResample *resample = 0;
    IVideoView *videoView = 0;
    IAudioPlay *audioPlay = 0;
protected:
    IPlayer(){};
    std::mutex mux;
    XParameter outPara;
};


#endif //FFMPEG_IPLAYER_H
