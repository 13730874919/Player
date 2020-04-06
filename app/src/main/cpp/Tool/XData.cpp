//
// Created by unionman on 2020/4/1.
//

#include "XData.h"


extern "C"{
#include <libavformat/avformat.h>
};

void XData::Drop()
{
    if(!data) return;
    av_packet_free((AVPacket **)&data);
    bool isAudio = false;
    data = 0;
    size = 0;

};