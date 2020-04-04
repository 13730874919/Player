//
// Created by unionman on 2020/4/1.
//

#include "XData.h"


extern "C"{
#include <libavformat/avformat.h>
};

void XData::drop()
{
    if(!data) return;
    av_packet_free((AVPacket **)&data);
    data = 0;
    size = 0;

};