package com.bds.ffmpeg;

import android.content.Context;
import android.util.Log;
import android.view.SurfaceHolder;

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
    public static UPlayer getInstance(){
        if(mplayer==null){
            mplayer = new UPlayer();
        }
        return mplayer;
    }
    public void stop(){
        Pause();
        status= false;
    }
    public void ause(){
        Pause();
        status= false;
    }
    public void release(){

    }
    public void setDataSource(Context text,String path){
        url=path;
    }
    public void prepare(){
         Open(url);
        Log.d("XPLAY","11111111111prepare");
    }
    public void start(){
        status= true;
        Start();
        Log.d("XPLAY","11111111111start");
    }
    public int  getCurrentPosition(){
        return  (int)GetPos();
    }
    public void seekTo(int pos){
         Seek( pos);
    }
    public void setDisplay(SurfaceHolder holder){
        Log.d("XPLAY","11111111111setDisplay");
        InitView(holder.getSurface());
    }
    private native void InitView(Object obj);
    public native void Pause();
    public native void play();
    public native void Open(String url);
    public native void Start();
    public native double GetPos();
    public native void Seek(double pos);
}
