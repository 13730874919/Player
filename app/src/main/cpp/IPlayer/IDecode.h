//
// Created by bds on 2020/4/6.
//

#ifndef FFMPEG_IDECODE_H
#define FFMPEG_IDECODE_H

#include <mutex>
#include <list>
#include "IObserver.h"
#include "XParameter.h"

class IDecode : public IObserver{
public:
    //打开解码器
    virtual bool Open(XParameter par)=0;
    virtual bool SendPacket(XData pkt)=0;
    //获取解码结果
    virtual XData RecvFrame()=0;

    void Update(XData data) override;

    bool isAudio = false;
    //最大的队列缓冲
    int maxList = 100;
protected:
    virtual void Main();
    std::mutex packsMutex;

    //读取缓冲
    std::list<XData> packs;
};


#endif //FFMPEG_IDECODE_H
