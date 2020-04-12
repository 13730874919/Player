//
// Created by bds on 2020/4/8.
//

#ifndef FFMPEG_IVIDEOVIEW_H
#define FFMPEG_IVIDEOVIEW_H


#include <IObserver.h>

class IVideoView : public IObserver{
public:
    void Update(XData data) override;
    virtual void setRender(void *win)=0;
    virtual void Render(XData data)=0;
    virtual void Close()=0;

};


#endif //FFMPEG_IVIDEOVIEW_H
