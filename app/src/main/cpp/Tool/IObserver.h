//
// Created by unionman on 2020/4/2.
//

#ifndef FFMPEG_IOBSERVER_H
#define FFMPEG_IOBSERVER_H


#include <mutex>
#include <vector>
#include "XData.h"
#include "XThread.h"

class IObserver : public XThread{
public:
    virtual void AddObs(IObserver *obs);
    virtual void Update(XData data){};
    virtual void Notify(XData data);

protected:
std::mutex mute;
std::vector<IObserver *> obbs;

};


#endif //FFMPEG_IOBSERVER_H
