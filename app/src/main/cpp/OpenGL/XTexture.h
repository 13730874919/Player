//
// Created by bds on 2020/4/8.
//

#ifndef FFMPEG_XTEXTURE_H
#define FFMPEG_XTEXTURE_H


#include "XShader.h"
enum XTextureType
{
    XTEXTURE_YUV420P = 0,  // Y 4  u 1 v 1
    XTEXTURE_NV12 = 25,    // Y4   uv1
    XTEXTURE_NV21 = 26     // Y4   vu1

};
class XTexture {
public:
    static XTexture *Get();
    virtual bool Init(void *win,XTextureType type=XTEXTURE_YUV420P) = 0;
    virtual void Draw(unsigned char *data[],int width,int height) = 0;
protected:
    XTexture(){};


};


#endif //FFMPEG_XTEXTURE_H
