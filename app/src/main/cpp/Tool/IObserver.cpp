//
// Created by unionman on 2020/4/2.
//

#include "IObserver.h"

void IObserver::AddObs(IObserver *obs) {
    if(!obs) return;
    mute.lock();
    obbs.push_back(obs);
    mute.unlock();
}


void IObserver::Notify(XData data) {
    mute.lock();
    for (int i = 0; i < obbs.size() ; i++) {
       obbs[i]->Update(data);
    }
    mute.unlock();
}