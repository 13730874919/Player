//
// Created by unionman on 2020/4/1.
//

#ifndef FFMPEG_XTHREAD_H
#define FFMPEG_XTHREAD_H


void XSleep(int mis);
class XThread {
public:
virtual bool start();
virtual void stop();
virtual void SetPause(bool isP);
virtual bool IsPause()
{
    isPausing = isPause;
    return isPause;
}
protected:
bool isPause = false;
bool isPausing = false;
bool isRunning=false;
bool isExit = false;
virtual void Main();

private:
    void threadMain();


};


#endif //FFMPEG_XTHREAD_H
