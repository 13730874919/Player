//
// Created by unionman on 2020/4/1.
//

#ifndef FFMPEG_XDATA_H
#define FFMPEG_XDATA_H


struct XData {
    unsigned char *data=0;
    int size=0;
    bool isAudio=false;
    int pts=0;
    void Drop();

};


#endif //FFMPEG_XDATA_H
