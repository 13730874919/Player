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
    if(type == PACK_TYPE)
        av_packet_free((AVPacket **)&data);
    else
        delete data;
    data = 0;
    size = 0;
    bool isAudio = false;
}

bool XData::Alloc(int size) {
    Drop();
    type = CHAR_TYPE;
    this->data = new unsigned char(size);
    if(!this->data) return false;
    return true;
};