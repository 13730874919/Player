
//
// Created by Administrator on 2018-03-07.
//

#ifndef XPLAY_IPLAYERPORXY_H
#define XPLAY_IPLAYERPORXY_H


#include "IPlayer.h"
#include <mutex>
class IPlayerPorxy: public IPlayer
{
public:
    static IPlayerPorxy*Get()
    {
        static IPlayerPorxy px;
        return &px;
    }
    void Init(void *vm = 0);

    double PlayPos() override;
    virtual bool Seek(double pos);
    virtual bool Open(const char *path);
    virtual void Close();
    virtual bool Start();
    virtual void InitView(void *win);
    virtual void SetPause(bool isP);
    virtual bool IsPause();
protected:
    IPlayerPorxy(){}
    IPlayer *player = 0;
    std::mutex mux;
};


#endif //XPLAY_IPLAYERPORXY_H
