//
// Created by unionman on 2020/4/10.
//

#ifndef FFMPEG_IRESAMPLE_H
#define FFMPEG_IRESAMPLE_H


#include <XParameter.h>
#include "IObserver.h"

class IResample: public IObserver{
public:
    virtual bool init(XParameter indata, XParameter outdata =  XParameter())=0;
    virtual XData Resample(XData indata)=0;
    void Update(XData data) override;
    int outChannels = 2;
    int outFormat = 1;
};


#endif //FFMPEG_IRESAMPLE_H
