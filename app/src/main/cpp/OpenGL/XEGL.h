//
// Created by bds on 2020/4/8.
//

#ifndef FFMPEG_XEGL_H
#define FFMPEG_XEGL_H


class XEGL {
public:
    virtual bool Init(void *win) = 0;
    static XEGL *Get();
    virtual void Draw() = 0;
    virtual void Close()=0;
protected:
    XEGL(){};
};


#endif //FFMPEG_XEGL_H
