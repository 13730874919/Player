package com.bds.ffmpeg;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.media.AudioManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.SeekBar;
import android.widget.TextView;

import com.bds.ffmpeg.universalvideoview.UniversalVideoView;

public class VideoViewActivity extends Activity implements Runnable, XPlay.XPlayOnclickLister,View.OnTouchListener{
    private final String TAG = "XPLAY";

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        int screenWidth = mVideoView.getWidth();
        int screenHeight = mVideoView.getHeight();
        Log.d(TAG,"screenWidth=="+screenWidth);
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                startX = event.getX();
                startY = event.getY();
                Log.d(TAG,"statx=="+startX);
                break;

            case MotionEvent.ACTION_MOVE:
                float endY = event.getY();
                float distanceY = startY - endY;
                if (startX > screenWidth / 2) {
//                    Log.d(TAG,"right");
//                    //屏幕左半部分上滑，亮度变大，下滑，亮度变小
//                  //  VolumTextView.setText(View.VISIBLE);
//                    Log.d(TAG,"left");
//                    final double FLING_MIN_DISTANCE = 0.5;
//                    final double FLING_MIN_VELOCITY = 0.5;
//                    if (distanceY > FLING_MIN_DISTANCE && Math.abs(distanceY) > FLING_MIN_VELOCITY) {
//                        setSccrenVolume(true);
//                    }
//                    if (distanceY < FLING_MIN_DISTANCE && Math.abs(distanceY) > FLING_MIN_VELOCITY) {
//                        setSccrenVolume(false);
//                    }
                } else {
                    //屏幕左半部分上滑，亮度变大，下滑，亮度变小
                  //  brightnessTextView.setText(View.VISIBLE);
                    Log.d(TAG,"left");
                    final double FLING_MIN_DISTANCE = 0.5;
                    final double FLING_MIN_VELOCITY = 0.5;
                    if (distanceY > FLING_MIN_DISTANCE && Math.abs(distanceY) > FLING_MIN_VELOCITY) {
                        setBrightness(10);
                    }
                    if (distanceY < FLING_MIN_DISTANCE && Math.abs(distanceY) > FLING_MIN_VELOCITY) {
                        setBrightness(-10);
                    }
                }
                break;
            case MotionEvent.ACTION_UP:
              //  brightnessTextView.setText(View.INVISIBLE);
                break;
        }
        return false;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {

        return super.onTouchEvent(event);
    }
    /*
     * 设置屏幕亮度
     * 0 最暗
     * 1 最亮
     */
    public void setBrightness(float brightness) {
        WindowManager.LayoutParams lp = getWindow().getAttributes();
        lp.screenBrightness = lp.screenBrightness + brightness / 255.0f;
        if (lp.screenBrightness > 1) {
            lp.screenBrightness = 1;
        } else if (lp.screenBrightness < 0.1) {
            lp.screenBrightness = (float) 0.1;
        }
        getWindow().setAttributes(lp);

        float sb = lp.screenBrightness;
        brightnessTextView.setVisibility(View.VISIBLE);
        brightnessTextView.setText((int) Math.ceil(sb * 100) + "%");
        mHandler.removeMessages(2);
        mHandler.sendEmptyMessageDelayed(2,1000);
    }
    /*
     * 设置屏幕亮度
     * 0 最暗
     * 1 最亮
     */
    public void setSccrenVolume(boolean bool) {
        mVolume = mAudioManager.getStreamVolume(AudioManager.STREAM_MUSIC);
        double val =0.0;
        if(bool){
            val= mVolume+0.1*mMaxVolume;
        }else{
            val= mVolume-0.1*mMaxVolume;
        }
        int ret=(int)val;
        if(val>mMaxVolume)ret=mMaxVolume;
        if(val<0)ret=0;
        Log.d(TAG, "ret==="+ret);
        mAudioManager.setStreamVolume(AudioManager.STREAM_MUSIC, ret, 0);
        VolumTextView.setText((int) Math.ceil(ret/mMaxVolume*100) + "%");
    }

    private float startY = 0;//手指按下时的Y坐标
    private float startX = 0;//手指按下时的Y坐标
    /** 最大声音 */
    private int mMaxVolume;
    /** 当前声音 */
    private int mVolume = -1;
    private AudioManager mAudioManager;
    /** 当前亮度 */
    private TextView brightnessTextView;
    private TextView VolumTextView;
    private View mVideoView;
    private SeekBar seek;
    private Thread th;
    private UniversalVideoView play;
    private UPlayer mplayer;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        requestWindowFeature(Window.FEATURE_NO_TITLE);//隐藏标题
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);//设置全屏
        mVideoView = LayoutInflater.from(this).inflate(R.layout.activity_main, null);
        setContentView(mVideoView);
        mplayer = new UPlayer();
        brightnessTextView= findViewById(R.id.light);
        VolumTextView= findViewById(R.id.volume);
        seek = findViewById( R.id.aplayseek );
        play = findViewById(R.id.myplay);
//        play.setOnTouchListener(this);
//        play.setXPlayOnclickLister(this);
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

        mAudioManager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
        mMaxVolume = mAudioManager
                .getStreamMaxVolume(AudioManager.STREAM_MUSIC);
        Intent intent = getIntent();
        String path =intent.getStringExtra("path");
        mplayer.setDataSource(this,path);
        mplayer.prepare();
        mplayer.start();
        //启动播放进度线程
        th = new Thread(this);
        th.start();
    }
    private Handler mHandler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            int what= msg.what;
            if(what==1) {
                mHandler.removeMessages(1);
                if (seek.getVisibility() == View.VISIBLE) {
                    seek.setVisibility(View.INVISIBLE);
                } else {
                    seek.setVisibility(View.VISIBLE);
                }
            }else if(what==2){
                brightnessTextView.setVisibility(View.INVISIBLE);
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
              //  PlayOrPause();
            }
        }
        startTime = SystemClock.elapsedRealtime();
    }

   // public native void PlayOrPause();
    public native void Open(String url);
    public native double PlayPos();
    public native void Seek(double pos);

    @Override
    public void run() {
        for(;;)
        {
            if(seek.getVisibility()==View.VISIBLE)
            seek.setProgress((int)(PlayPos()*1000));
            try {
                Thread.sleep( 100 );
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }
}
