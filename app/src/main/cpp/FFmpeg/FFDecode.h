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
    void Close() override;

    virtual bool Open(XParameter par,bool isHard= false);
    virtual bool SendPacket(XData pkt);
    //从线程中获取解码结果，再次调用会复用上次空间，线程不安全
    virtual XData RecvFrame();

    void Clear(bool isClearPts=false) override;

    static void InitHard(void *vm);
protected:
    AVCodecContext *codec = 0;
    AVFrame *frame = 0;
    int iflag = 0;
    std::mutex mux;
};


#endif //FFMPEG_FFDECODE_H
