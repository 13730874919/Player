package com.bds.ffmpeg;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.SeekBar;

public class MainActivity extends Activity implements Runnable, XPlay.XPlayOnclickLister{

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    public void run() {
        for(;;)
        {
            seek.setProgress((int)(PlayPos()*1000));
            try {
                Thread.sleep( 40 );
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }

    private SeekBar seek;
    private Thread th;
    private XPlay play;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);//隐藏标题
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);//设置全屏
        setContentView(R.layout.activity_main);
        seek = findViewById( R.id.aplayseek );
        play = findViewById(R.id.myplay);
        play.setXPlayOnclickLister(this);
        seek.setMax(1000);
        seek.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                Seek( (double)seekBar.getProgress()/(double)seekBar.getMax() );
            }
        });
        Intent intent = getIntent();
        String path =intent.getStringExtra("path");
        Open(path);
        //启动播放进度线程
        th = new Thread(this);
        th.start();
    }
    private Handler mHandler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            mHandler.removeMessages(1);
            if(seek.getVisibility()==View.VISIBLE) {
                seek.setVisibility(View.INVISIBLE);
            }else {
                seek.setVisibility(View.VISIBLE);
            }
        }
    };
    private  long startTime =0;
    @Override
    public void onXPlayOnclick() {
        mHandler.sendEmptyMessageDelayed(1,200);
        if(startTime!=0) {
            long timer = (SystemClock.elapsedRealtime() - startTime);
            if(timer<200){
                mHandler.removeMessages(1);
                PlayOrPause();
            }
        }
        startTime = SystemClock.elapsedRealtime();
    }

    public native void PlayOrPause();
    public native void Open(String url);
    public native double PlayPos();
    public native void Seek(double pos);
}
