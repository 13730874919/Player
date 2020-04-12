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
    virtual bool Open(XParameter par,bool isHard= false)=0;
    virtual bool SendPacket(XData pkt)=0;
    //获取解码结果
    virtual XData RecvFrame()=0;
    virtual void Close()=0;
    virtual void Clear();
    void Update(XData data) override;

    bool isAudio = false;
    //最大的队列缓冲
    int maxList = 100;

    //同步时间，再次打开文件要清理
    int synPts = 0;
    int pts = 0;
protected:
    virtual void Main();
    std::mutex packsMutex;

    //读取缓冲
    std::list<XData> packs;
};


#endif //FFMPEG_IDECODE_H
