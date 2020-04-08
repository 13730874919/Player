//
// Created by bds on 2020/4/8.
//

#ifndef FFMPEG_GLVIDEOVIEW_H
#define FFMPEG_GLVIDEOVIEW_H


#include "IVideoView.h"
#include "XTexture.h"

class GLVideoView : public IVideoView{
public:
    virtual void setRender(void *win);
    virtual void Render(XData data);

protected:
    void *view = 0;
    XTexture *txt = 0;
};


#endif //FFMPEG_GLVIDEOVIEW_H
