//
// Created by unionman on 2020/4/2.
//

#ifndef FFMPEG_IOBSERVER_H
#define FFMPEG_IOBSERVER_H


#include <mutex>
#include <vector>
#include "XData.h"

class IObserver {
public:
    virtual void AddObs();
    virtual void Update();
    virtual void Notifly();

protected:
std::mutex mute;
std::vector<IObserver *> obbs;



};


#endif //FFMPEG_IOBSERVER_H
