//
// Created by unionman on 2020/4/1.
//

#ifndef FFMPEG_IDEMUX_H
#define FFMPEG_IDEMUX_H

#include <IObserver.h>
#include <XParameter.h>
#include "XData.h"
#include "XThread.h"


class IDemux: public IObserver {
public:
    virtual bool Open(const  char *url)=0;
    virtual XData Read()=0;
    virtual XParameter GetVPara()=0;
    virtual XParameter GetAPara()=0;
    //seek 位置 pos 0.0~1.0
    virtual bool Seek(double pos)=0;
    virtual void Close()=0;
    int total=0;
protected:

    virtual void Main();
};


#endif //FFMPEG_IDEMUX_H
