//
// Created by unionman on 2020/4/1.
//

#ifndef FFMPEG_FFDEMUX_H
#define FFMPEG_FFDEMUX_H


#include "IDemux.h"
struct AVFormatContext;
class FFDemux: public IDemux {
public:
    virtual bool Open(const  char *url);
    virtual XParameter GetVPara();
    virtual XData Read();
    virtual XParameter GetAPara() override;

    FFDemux();
private:
    AVFormatContext *ic=0;
    int audioStream = 1;
    int videoStream = 0;
};


#endif //FFMPEG_FFDEMUX_H
