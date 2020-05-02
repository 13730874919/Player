package com.bds.ffmpeg;

import android.content.Context;
import android.util.Log;
import android.view.SurfaceHolder;

public class UPlayer {
    public UPlayer() {
        super();
    }
    private String url = "/sdcard/1080.mp4";
    private int Duration=0;
    final int PREPARE=0;
    final int PLAYING=0;
    final int IDEL=0;
    final int PAUSE=0;

    static {
        System.loadLibrary("native-lib");
    }
    private static UPlayer mplayer;
    public static UPlayer getInstance(){
        if(mplayer==null){
            mplayer = new UPlayer();
        }
        return mplayer;
    }
    public void stop(){
        Pause();
    }
    public void pause(){
        Log.d("XPLAY","pausepausepause");
        Pause();
    }
    public void release(){

    }
    public void setDataSource(Context text,String path){
        url=path;
    }
    public void prepare(){
         Open(url);
        Duration = native_getDuration();
       Log.e("XPLAY",Log.getStackTraceString(new Throwable()));
    }
    public void start(){
        if(native_getPlayerStatus()==PREPARE)
          Start();
        else play();
        if(mOnPreparedListener!=null){
            mOnPreparedListener.onPrepared(mplayer);
        }
        Log.e("XPLAY",Log.getStackTraceString(new Throwable()));
      //  Log.d("XPLAY","11111111111start");
    }
    public int getDuration(){
        int duration = native_getDuration();
        return duration;
    }
    public int  getCurrentPosition(){
        Double ppos =  GetPos();
       int pos =  (int)(ppos*Duration);
        return  pos;
    }

    public void seekTo(int pos){
        Log.d("XPLAY","11111111111seekTo="+pos);
        double dpos = pos/(double)Duration;
        Log.d("XPLAY","11111111111seekTo="+dpos);
        Seek( dpos);
    }
    public void setDisplay(SurfaceHolder holder){
        Log.d("XPLAY","11111111111setDisplay");
        InitView(holder.getSurface());
    }
    public boolean isPlaying(){
        boolean bool = native_isPlaying();
        Log.d("XPLAY","11111111111isPlaying"+bool);
        return bool;
    }
    public int getVideoWidth(){
        return native_getVideoWidth();
    }
    public int getVideoHeight(){
        return native_getVideoHeight();
    }
    private native void InitView(Object obj);
    public native void Pause();
    public native void play();
    public native void Open(String url);
    public native void Start();
    public native int native_getDuration();
    public native double GetPos();
    public native void Seek(double pos);
    public native boolean native_isPlaying();
    public native int native_getPlayerStatus();
    public native int native_getVideoHeight();
    public native int native_getVideoWidth();

    private OnPreparedListener mOnPreparedListener;
    public void setOnPreparedListener(OnPreparedListener p){
        mOnPreparedListener = p;
    }
    public interface OnPreparedListener {
         void onPrepared(UPlayer mp);
    }

}
