//
// Created by unionman on 2020/4/10.
//

#include <XLog.h>
#include "IResample.h"

void IResample::Update(XData data) {

     XData retData = Resample(data);
     if(retData.size>0){
         this->Notify(retData);
     }
}

