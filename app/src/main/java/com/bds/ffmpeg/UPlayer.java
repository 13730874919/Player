package com.bds.ffmpeg;

import android.content.Context;
import android.util.Log;
import android.view.SurfaceHolder;

public class UPlayer {
    private final String TAG = "UPlayer";
    public UPlayer() {
        super();
    }
    private String url = "/sdcard/1080.mp4";
    private int Duration=0;
    final int PREPARE=0;
    final int PLAYING=1;
    final int IDEL=2;
    final int PAUSE=3;

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
        Log.e(TAG,"Uplayer stop Pause");
        Pause();
    }
    public void pause(){
        Log.e(TAG,"Uplayer Pause");
        Log.e(TAG,Log.getStackTraceString(new Throwable()));
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
        Log.e(TAG,"Uplayer prepare");
    }
    public void start(){
        if(native_getPlayerStatus()==PREPARE){
            Log.e(TAG,"Uplayer Start");
            Start();
        }
        else{
            Log.e(TAG,"Uplayer play");
            play();
        }
        if(mOnPreparedListener!=null){
            mOnPreparedListener.onPrepared(mplayer);
        }
    }
    public int getDuration(){
        int duration = native_getDuration();
    //    Log.e(TAG,"Uplayer getDuration");
        return duration;
    }
    public int  getCurrentPosition(){
        Double ppos =  GetPos();
       int pos =  (int)(ppos*Duration);
    //    Log.e(TAG,"Uplayer getCurrentPosition");
        return  pos;
    }

    public void seekTo(int pos){
        double dpos = pos/(double)Duration;
        Log.e(TAG,"Uplayer seekTo");
        Seek( dpos);
    }
    public void setDisplay(SurfaceHolder holder){
        Log.e(TAG,"Uplayer setDisplay");
        InitView(holder.getSurface());
    }
    public boolean isPlaying(){
        boolean bool =false;
        if(native_getPlayerStatus()==PLAYING)
        {
              bool= true;
        }
      //  Log.e(TAG,"Uplayer isPlaying"+bool);
        return bool;
    }
    public int getVideoWidth(){
     //   Log.e(TAG,"Uplayer getVideoWidth");
        return native_getVideoWidth();
    }
    public int getVideoHeight(){
      //  Log.e(TAG,"Uplayer getVideoHeight");
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
