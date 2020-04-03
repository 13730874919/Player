//
// Created by unionman on 2020/4/1.
//

#ifndef FFMPEG_IDEMUX_H
#define FFMPEG_IDEMUX_H

#include "XData.h"
#include "XThread.h"


class IDemux: public XThread {
public:
    virtual bool Open(const  char *url)=0;
    virtual XData Read()=0;
    int total=0;
protected:

    virtual void Main();
};


#endif //FFMPEG_IDEMUX_H
