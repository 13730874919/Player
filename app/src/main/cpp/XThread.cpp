//
// Created by unionman on 2020/4/1.
//

#include "XThread.h"
#include "XLog.h"
#include <thread>
using namespace std;
void XSleep(int mis){
    chrono::milliseconds du(mis);
    this_thread::sleep_for(du);

}

void XThread::start() {

    isExit= false;
    thread th(&XThread::threadMain,this);
    th.detach();
}

void XThread::stop() {
    XLOGI("stop thread");
    isExit = true;
    for(int i=0;i<200;i++){
        if(!isRunning) {
             XLOGI("stop success");
             return;
        }
        XSleep(1);
    }
    XLOGI(" thread end");
}

void XThread::threadMain() {
    isRunning= true;
    XLOGI("thread start");
    Main();
    XLOGI("thread end");
    isRunning= false;
}

void XThread::Main() {

}
