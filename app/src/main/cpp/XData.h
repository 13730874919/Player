//
// Created by unionman on 2020/4/1.
//

#ifndef FFMPEG_XDATA_H
#define FFMPEG_XDATA_H


struct XData {
    unsigned char *data;
    int size=0;
    void drop();

};


#endif //FFMPEG_XDATA_H
