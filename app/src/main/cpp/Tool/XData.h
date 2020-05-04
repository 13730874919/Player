//
// Created by unionman on 2020/4/1.
//

#ifndef FFMPEG_XDATA_H
#define FFMPEG_XDATA_H

enum DATA_TYPE{
    PACK_TYPE=0,
    CHAR_TYPE=1,
};
struct XData {
    int type = PACK_TYPE;
    int format = 0;
    unsigned char *data=0;
    int width = 0;
    int height =0;
    unsigned char *framedatas[8]={0};
    int size=0;
    bool isAudio=false;
    int pts=0;
    int dts=0;
    bool Alloc(int size,const char *d=0);
    void Drop();

};


#endif //FFMPEG_XDATA_H
