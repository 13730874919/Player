package com.bds.ffmpeg;

import android.content.Context;

public class UPlayer {
    public UPlayer() {
        super();
    }
    private String url = "/sdcard/1080.mp4";
    static {
        System.loadLibrary("native-lib");
    }
    private static UPlayer mplayer;
    private boolean status = false;
    static UPlayer getInstance(){
        if(mplayer==null){
            mplayer = new UPlayer();
        }
        return mplayer;
    }
    public void stop(){
        Pause();
        status= false;
    }
    public void release(){

    }
    public void setDataSource(Context text,String path){
        url=path;
    }
    public void prepare(){

    }
    public void start(){
        status= true;
        Start();
    }
    public void getposition(){

    }
    public void seekTo(){

    }
    private native void InitView(Object obj);
    public native void Pause();
    public native void play();
    public native void Open(String url);
    public native void Start();
    public native double GetPos();
    public native void Seek(double pos);
}
