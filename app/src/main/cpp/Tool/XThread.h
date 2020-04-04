//
// Created by unionman on 2020/4/1.
//

#ifndef FFMPEG_XTHREAD_H
#define FFMPEG_XTHREAD_H

void XSleep(int mis);
class XThread {
public:
virtual void start();
virtual void stop();

protected:
bool isRunning=false;
bool isExit = true;
virtual void Main();

private:
    void threadMain();


};


#endif //FFMPEG_XTHREAD_H
