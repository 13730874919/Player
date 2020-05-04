//
// Created by bds on 2020/4/6.
//

#include <XLog.h>
#include "FFDecode.h"
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavcodec/jni.h>
}
 void FFDecode::InitHard(void *vm)
{
    av_jni_set_java_vm(vm,0);
}
bool FFDecode::Open(XParameter par,bool isHard){
    Close();
    if(!par.para)return false;
    AVCodecParameters *p = par.para;
    AVCodec *cd = avcodec_find_decoder(p->codec_id);

    if(isHard)
    {
        if(p->codec_type==AVMEDIA_TYPE_VIDEO){
            XLOGE("avcodec_find_decoder %d ",p->codec_id);
            XLOGE("AV_CODEC_ID_H264 %d ",AV_CODEC_ID_H264);
            cd = avcodec_find_decoder_by_name("h264_mediacodec");
        }

    }
    if(!cd)
    {
        XLOGE("avcodec_find_decoder %d failed!",p->codec_id);
        return false;
    }
    XLOGI("avcodec_find_decoder success!");
    mux.lock();
    //创建上下文
    codec = avcodec_alloc_context3(cd);
    avcodec_parameters_to_context(codec,p);
    codec->thread_count = 8;
    //打开解码器
    int re = avcodec_open2(codec,0,0);
    if(re != 0)
    {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re,buf,sizeof(buf)-1);
        XLOGE("%s",buf);
        return false;
    }
    if(codec->codec_type==AVMEDIA_TYPE_VIDEO){
        this->isAudio = false;
    }else if(codec->codec_type==AVMEDIA_TYPE_AUDIO){
        this->isAudio = true;
    }else{
        mux.unlock();
        return false;
        XLOGI("error codec_type exit!");
    }
    mux.unlock();
    XLOGI("avcodec_open2 success!");
    return true;
}

static int reccnt = 0;
bool FFDecode::SendPacket(XData pkt) {
    if(pkt.size<=0 || !pkt.data)return false;
    mux.lock();
    if(!codec)
    {
        mux.unlock();
        return false;
    }
    int re = avcodec_send_packet(codec,(AVPacket*)pkt.data);

    if(re != 0)
    {
        mux.unlock();
        XLOGI(" avcodec_send_packet error ret=%d", re);
      //  XLOGI(" avcodec_send_packet error sendcnt=%d ",sendcnt);
        return false;
    }

    mux.unlock();
    return true;
}
static double r2d(AVRational r)
{
    return r.num == 0 || r.den == 0 ?0.:(double) r.num/(double)r.den;
}
XData FFDecode::RecvFrame() {
    mux.lock();
    if (!codec) {
        mux.unlock();
        return XData();
    }
    if (!frame) {
        frame = av_frame_alloc();
    }
    int re = avcodec_receive_frame(codec, frame);

    if (re != 0) {
        mux.unlock();
        return XData();
    }

    XData d;

    d.data = (unsigned char *) frame;
    if (codec->codec_type == AVMEDIA_TYPE_VIDEO){
        //YUV
        d.size = (frame->linesize[0] + frame->linesize[1] + frame->linesize[2]) * frame->height;
        d.width = frame->width;
        d.height = frame->height;
    }
    else {
        //样本字节数 * 单通道样本数 * 通道数
        d.size = av_get_bytes_per_sample((AVSampleFormat)frame->format)*frame->nb_samples*2;
    }

    d.format = frame->format;
    d.pts = frame->pts;
    memcpy(d.framedatas,frame->data,sizeof(d.framedatas));
    mux.unlock();
    return d;
}

void FFDecode::Close() {
    IDecode::Clear();
    mux.lock();
    iflag=0;
    XLOGE("frame.Close");
    pts = 0;
    if(frame)
        av_frame_free(&frame);
    if(codec)
    {
        avcodec_close(codec);
        avcodec_free_context(&codec);
    }
    mux.unlock();
}

void FFDecode::Clear(bool isClearPts) {
    IDecode::Clear(isClearPts);
    mux.lock();
    iflag=0;
    if(codec)
        avcodec_flush_buffers(codec);
    mux.unlock();
}
