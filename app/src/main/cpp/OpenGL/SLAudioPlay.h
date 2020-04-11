//
// Created by unionman on 2020/4/10.
//

#ifndef FFMPEG_SLAUDIOPLAY_H
#define FFMPEG_SLAUDIOPLAY_H


#include <IAudioPlay.h>

class SLAudioPlay : public IAudioPlay {
public:
    bool StartPlay(XParameter out) override;
    void PlayCall(void *bufq);

    SLAudioPlay();
    virtual ~SLAudioPlay();
protected:
    unsigned char *buf = 0;

};


#endif //FFMPEG_SLAUDIOPLAY_H
