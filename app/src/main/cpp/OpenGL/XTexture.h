//
// Created by bds on 2020/4/8.
//

#ifndef FFMPEG_XTEXTURE_H
#define FFMPEG_XTEXTURE_H


#include "XShader.h"

class XTexture {
public:
    static XTexture *Get();
    virtual  bool init(void *win)=0;
    virtual void Draw(unsigned char *data[],int width,int height) = 0;
protected:
    XTexture(){};


};


#endif //FFMPEG_XTEXTURE_H
