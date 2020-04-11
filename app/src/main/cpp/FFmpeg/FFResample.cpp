//
// Created by unionman on 2020/4/10.
//
extern "C"{
#include <libavutil/samplefmt.h>
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
}

#include <XLog.h>
#include "FFResample.h"


XData FFResample::Resample(XData indata) {
    if(indata.size<=0 || !indata.data) return XData();
    if(!actx)
        return XData();
    //XLOGE("indata size is %d",indata.size);
    AVFrame *frame = (AVFrame *)indata.data;

    //输出空间的分配
    XData out;
    int outsize = outChannels * frame->nb_samples * av_get_bytes_per_sample((AVSampleFormat)outFormat);
    if(outsize <=0)return XData();
    out.Alloc(outsize);
    uint8_t *outArr[2] = {0};
    outArr[0] = out.data;
    int len = swr_convert(actx,outArr,frame->nb_samples,(const uint8_t **)frame->data,frame->nb_samples);
    if(len<=0)
    {
        out.Drop();
        return XData();
    }
    //XLOGE("swr_convert success = %d",len);
    return out;
}

bool FFResample::init(XParameter indata, XParameter outdata) {
    //音频重采样上下文初始化
    actx = swr_alloc();
    actx = swr_alloc_set_opts(actx,
                              av_get_default_channel_layout(outdata.channels),
                              AV_SAMPLE_FMT_S16,outdata.sample_rate,
                              av_get_default_channel_layout(indata.para->channels),
                              (AVSampleFormat)indata.para->format,indata.para->sample_rate,
                              0,0 );

    int re = swr_init(actx);
    if(re != 0)
    {
        XLOGE("swr_init failed!");
        return false;
    }
    else
    {
        XLOGI("swr_init success!");
    }
    outChannels = indata.para->channels;
    outFormat = AV_SAMPLE_FMT_S16;
    return true;
}
