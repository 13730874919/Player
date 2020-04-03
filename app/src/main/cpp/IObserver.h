//
// Created by unionman on 2020/4/2.
//

#ifndef FFMPEG_IOBSERVER_H
#define FFMPEG_IOBSERVER_H

#include "XData.h"
#include "XThread.h"

class IObserver {
public:
    virtual void AddObs();
    virtual void Update();
    virtual void Notifly();

protected:
    std::vector <IObserver *> obss;


};


#endif //FFMPEG_IOBSERVER_H
