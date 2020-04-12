//
// Created by bds on 2020/4/11.
//

#ifndef FFMPEG_FFPLAYERBUILDER_H
#define FFMPEG_FFPLAYERBUILDER_H


#include <IPlayerBuilder.h>
#include <IPlayer.h>

class FFPlayerBuilder: public IPlayerBuilder {
public:
    static void InitHard(void *vm);
    static FFPlayerBuilder *Get()
    {
        static FFPlayerBuilder ff;
        return &ff;
    }
protected:
    FFPlayerBuilder(){};
    IDemux *CreateDemux() override;

    IDecode *CreateDecode() override;

    IAudioPlay *CreateAudioPlay() override;

    IPlayer *CreatePlayer(unsigned char index) override;

    IVideoView *CreateVideoView() override;

    IResample *CreateResample() override;
};


#endif //FFMPEG_FFPLAYERBUILDER_H
