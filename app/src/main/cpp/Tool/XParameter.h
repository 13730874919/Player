//
// Created by bds on 2020/4/6.
//

#ifndef FFMPEG_XPARAMETER_H
#define FFMPEG_XPARAMETER_H

struct AVCodecParameters;
class XParameter {
public:
    AVCodecParameters *para=0;
    int channels = 2;
    int sample_rate = 44100;

};


#endif //FFMPEG_XPARAMETER_H
