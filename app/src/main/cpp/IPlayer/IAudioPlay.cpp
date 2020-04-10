//
// Created by unionman on 2020/4/10.
//

#include <XLog.h>
#include "IAudioPlay.h"

void IAudioPlay::Update(XData data) {
    XLOGE("IAudioPlay::Update %d",data.size);
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

