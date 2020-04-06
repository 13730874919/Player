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
        packsMutex.lock();
        if (packs.empty())
        {
            packsMutex.unlock();
            XSleep(1);
            continue;
        }
        //取出packet 消费者
        XData pack = packs.front();
        packs.pop_front();
        XLOGI("FFDecode pop_front once ");
        //发送数据到解码线程，一个数据包，可能解码多个结果
        if (this->SendPacket(pack))
        {
            while (!isExit)
            {
                //获取解码数据
                XData frame = RecvFrame();
                if (!frame.data) break;
                //XLOGE("RecvFrame %d",frame.size);
            //    XLOGI("IDecode Read pts==%d",frame.pts);
                //发送数据给观察者
                this->Notify(frame);
            }
        }
        pack.Drop();
        packsMutex.unlock();
    }
}
