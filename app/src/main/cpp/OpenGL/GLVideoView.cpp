//
// Created by bds on 2020/4/8.
//

#include "GLVideoView.h"

void GLVideoView::setRender(void *win) {
    view = win;
}

void GLVideoView::Render(XData data) {
    if(!view) return;
    if(!txt){
        txt = XTexture::Get();
        txt->Init(view,XTextureType(data.format));
    }
    txt->Draw(data.framedatas,data.width,data.height);
}

void GLVideoView::Close() {
    mux.lock();
    if(txt)
    {
        txt->Drop();
        txt = 0;
    }

    mux.unlock();
}
