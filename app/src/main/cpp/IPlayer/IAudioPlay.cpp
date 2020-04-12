//
// Created by unionman on 2020/4/10.
//

#include <XLog.h>
#include "IAudioPlay.h"

void IAudioPlay::Update(XData data) {
    //压入缓冲队列
    if(data.size<=0|| !data.data) return;
    while(!isExit)
    {
        frameMute.lock();
        if(frames.size() > maxFrame)
        {
            frameMute.unlock();
            XSleep(1);
            continue;
        }
        frames.push_back(data);
        frameMute.unlock();
        break;
    }
}

XData IAudioPlay::GetData() {
    XData d;
    while(!isExit)
    {
        frameMute.lock();
        if(!frames.empty())
        {
            d = frames.front();
            frames.pop_front();
            frameMute.unlock();
            pts = d.pts;
            return d;
        }
        frameMute.unlock();
        XSleep(1);
    }
    return d;
}

