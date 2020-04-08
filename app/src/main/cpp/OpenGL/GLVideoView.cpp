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
        XTexture::Get()->init(view);
    }
    txt->Draw(data.framedatas,data.width,data.height);
}
