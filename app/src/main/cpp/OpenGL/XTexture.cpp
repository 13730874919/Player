//
// Created by bds on 2020/4/8.
//

#include <XLog.h>
#include "XTexture.h"
#include "XEGL.h"

class CXTexture : public XTexture{
public:
    std::mutex mux;
    void Drop() override {
        mux.lock();
        XEGL::Get()->Close();
        sh.Close();
        mux.unlock();
        delete this;
    }

    XShader sh;
    XTextureType type;
    void Draw(unsigned char **data, int width, int height) override {
        mux.lock();
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
        mux.unlock();
    }

    bool Init(void *win, XTextureType type) override {
        mux.lock();
        XEGL::Get()->Close();
        sh.Close();
        this->type = type;
        if(!win){
            mux.unlock();
            return false;
        }
        bool  ret = XEGL::Get()->Init(win);
        if(!ret)
        {
            mux.unlock();
            XLOGE("XTexture Init failed win is NULL");
            return false;
        }
        ret = sh.Init((XShaderType)type);
        if(!ret)
        {
            mux.unlock();
            XLOGE("XTexture shader Init faile");
            return false;
        }
        mux.unlock();
        return true;
    }
};
XTexture *XTexture::Get() {
       static CXTexture text;
       return &text;
}

XTexture::~XTexture() {}

