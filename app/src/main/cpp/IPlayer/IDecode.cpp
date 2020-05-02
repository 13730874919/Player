//
// Created by bds on 2020/4/6.
//

#include <XLog.h>
#include "IDecode.h"

//设置缓冲队列，超过阻塞
void IDecode::Update(XData data) {
    if(data.isAudio != isAudio)
    {
        return;
    }

    while(!isExit){
        packsMutex.lock();
        if(packs.size()<maxList){
            packs.push_back(data);
            packsMutex.unlock();
            break;
        }
        packsMutex.unlock();
        XSleep(1);
    }
}


void IDecode::Main()
{
    while(!isExit)
    {
        if(IsPause())
        {
            XSleep(2);
            continue;
        }
        packsMutex.lock();
        //判断音视频同步
        if(!isAudio && synPts > 0)
        {
            if(synPts < pts)
            {
                packsMutex.unlock();
                XSleep(1);
                continue;
            }
        }

        if (packs.empty())
        {
            packsMutex.unlock();
            XSleep(1);
            continue;
        }
        //取出packet 消费者
        XData pack = packs.front();
        packs.pop_front();
        //发送数据到解码线程，一个数据包，可能解码多个结果
        if (this->SendPacket(pack))
        {
            while (!isExit)
            {
                //获取解码数据
                XData frame = RecvFrame();
                if (!frame.data) break;
           //     XLOGE("RecvFrame %d",frame.size);
                //发送数据给观察者
                if(frame.pts==0){
                    XLOGE("frame.pts %d",frame.pts);
                }
                pts= frame.pts;
                this->Notify(frame);
            }
        }
        pack.Drop();
        packsMutex.unlock();
    }
}

void IDecode::Clear(bool isClearPts) {
    packsMutex.lock();
    while(!packs.empty())
    {
        packs.front().Drop();
        packs.pop_front();
    }
    if(!isClearPts){
        pts = 0;
    }
    synPts = 0;
    packsMutex.unlock();
}
