//
// Created by bds on 2020/4/8.
//

#include <XLog.h>
#include "XTexture.h"
#include "XEGL.h"

class CXTexture : public XTexture{
public:
    XShader sh;

    void Draw(unsigned char **data, int width, int height) override {
           sh.GetTexture(0,width,height,data[0]);  // Y
           sh.GetTexture(1,width/2,height/2,data[1]);  // U
           sh.GetTexture(2,width/2,height/2,data[2]);  // V
           sh.Draw();
           XEGL::Get()->Draw();
    }

    bool init(void *win) override {
        if(!win)return false;
        bool  ret = XEGL::Get()->Init(win);
        if(!ret)
        {
            XLOGE("XTexture Init failed win is NULL");
            return false;
        }
        ret = sh.Init();
        if(!ret)
        {
            XLOGE("XTexture shader Init faile");
            return false;
        }
        return true;

    }
};
XTexture *XTexture::Get() {
    CXTexture text;
    return &text;
}

