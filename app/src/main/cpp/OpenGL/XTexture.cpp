//
// Created by bds on 2020/4/8.
//

#include <XLog.h>
#include "XTexture.h"
#include "XEGL.h"

class CXTexture : public XTexture{
public:
    XShader sh;
    XTextureType type;
    void Draw(unsigned char **data, int width, int height) override {
           sh.GetTexture(0,width,height,data[0]);  // Y
        if(type == XTEXTURE_YUV420P)
        {
            sh.GetTexture(1,width/2,height/2,data[1]);  // U
            sh.GetTexture(2,width/2,height/2,data[2]);  // V
        }
        else
        {
            sh.GetTexture(1,width/2,height/2,data[1], true);  // UV
        }
           sh.Draw();
           XEGL::Get()->Draw();
    }

    bool Init(void *win, XTextureType type) override {
        this->type = type;
        if(!win)return false;
        bool  ret = XEGL::Get()->Init(win);
        if(!ret)
        {
            XLOGE("XTexture Init failed win is NULL");
            return false;
        }
        ret = sh.Init((XShaderType)type);
        if(!ret)
        {
            XLOGE("XTexture shader Init faile");
            return false;
        }
        return true;
    }
};
XTexture *XTexture::Get() {
       static CXTexture text;
       return &text;
}

