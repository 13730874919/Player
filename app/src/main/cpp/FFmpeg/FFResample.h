//
// Created by unionman on 2020/4/10.
//

#ifndef FFMPEG_FFRESAMPLE_H
#define FFMPEG_FFRESAMPLE_H


#include <IResample.h>
struct SwrContext;
class FFResample : public IResample{
public:
    bool init(XParameter indata, XParameter outdata) override;

    XData Resample(XData indata) override;

protected:
    SwrContext * actx;
};


#endif //FFMPEG_FFRESAMPLE_H
