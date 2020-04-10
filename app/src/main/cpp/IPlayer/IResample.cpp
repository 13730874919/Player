//
// Created by unionman on 2020/4/10.
//

#include "IResample.h"

void IResample::Update(XData data) {
    if(!data.data) return;
     XData retData = Resample(data);
     if(retData.size>0){
         this->Notify(retData);
     }
}

