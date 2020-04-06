//
// Created by bds on 2020/4/6.
//

#ifndef FFMPEG_FFDECODE_H
#define FFMPEG_FFDECODE_H


#include "IDecode.h"

struct  AVCodecContext;
struct AVFrame;
class FFDecode : public IDecode{
public:
    virtual bool Open(XParameter par);
    virtual bool SendPacket(XData pkt);
    //从线程中获取解码结果，再次调用会复用上次空间，线程不安全
    virtual XData RecvFrame();
protected:
    AVCodecContext *codec = 0;
    AVFrame *frame = 0;
};


#endif //FFMPEG_FFDECODE_H
