package com.bds.ffmpeg.universalvideoview;

import android.app.Activity;
import android.app.Service;
import android.media.AudioManager;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.SeekBar;
import android.widget.TextView;

import com.bds.ffmpeg.R;

/**
 * Created by Mai on 2018/8/7
 * *
 * Description:
 * *
 */
public class VideoGestureListener extends GestureDetector.SimpleOnGestureListener implements View.OnTouchListener {

    private UniversalMediaController mControlPanel;
    private float currentX;
    private float currentY;
    private float currentWidth;
    private float currentHeight;
    private float baseValue; //缩放时，两指间初始距离
    private int mVolume = -1;//当前声音
    private int mMaxVolume;//最大声音
    private float mBrightness = -1f;//当前亮度
    private AudioManager mAudioManager;
    public ProgressBar pbOperation;//调节音量
    public ImageView imgOperation;//
    public LinearLayout llOperation;

    private boolean firstTouch;//按住屏幕不放的第一次点击，则为true
    private boolean mChangePosition;//判断是改变进度条则为true，否则为false
    private boolean mChangeBrightness=false;//判断是不是改变亮度的操作
    private boolean mChangeVolume=false;//判断是不是改变音量的操作
    private SeekBar seekBar;
    private int preDuration;//手势调整进度条的滑动距离

    private LinearLayout llProgressTime;//展示手势滑动进度条的图层
    private TextView tvProgressTime;//展示手势滑动改变多少的进度条


    private VideoGestureListener() {
    }

    public VideoGestureListener(UniversalMediaController controlPanel) {
        mControlPanel = controlPanel;
        llOperation = mControlPanel.findViewById(R.id.llOperation);
        pbOperation = mControlPanel.findViewById(R.id.pbOperation);
        imgOperation = mControlPanel.findViewById(R.id.imgOperation);
        mAudioManager = (AudioManager) mControlPanel.getContext().getSystemService(Service.AUDIO_SERVICE);
        mMaxVolume = mAudioManager.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
        seekBar = mControlPanel.findViewById(R.id.seekbar);
        llProgressTime = mControlPanel.findViewById(R.id.llProgressTime);
        tvProgressTime = mControlPanel.findViewById(R.id.tvProgressTime);
        target = mControlPanel.getTarget();
    }
    private   UniversalVideoView target;
    private Boolean isproChange=false;
    @Override
    public boolean onDown(MotionEvent e) {
        target = mControlPanel.getTarget();
        if (target == null) return false;
        baseValue = 0;
        currentX = target.getX();
        currentY = target.getY();
        currentWidth = target.getWidth();
        currentHeight = target.getHeight();

        //取消隐藏音量和亮度的图层的操作
        llOperation.getHandler().removeCallbacks(runnable);
        firstTouch = true;
        mChangeVolume=false;
        mChangeBrightness=false;
        mChangePosition = false;
        return true;
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e) {
        mControlPanel.performClick();
        return false;
    }

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
        return super.onFling(e1, e2, velocityX, velocityY);
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {

            if (e2.getPointerCount() == 1) {//单指移动
                float mOldX = e1.getX(), mOldY = e1.getY();
                int x = (int) e2.getRawX();
                int y = (int) e2.getRawY();
                if (firstTouch) {
                    mChangePosition = Math.abs(distanceX) >= Math.abs(distanceY);
                    if (!mChangePosition) {
                        if (mOldX > currentWidth * 2.0 / 3) {
                            //右边三分之一区域滑动
                            mChangeVolume = true;
                        } else if ((mOldX < currentWidth / 3.0)) {
                            //左边三分之一区域滑动
                            mChangeBrightness = true;
                        }
                    }
                    firstTouch = false;
                }

                if (mChangePosition) {
                   onSeekProgressControl(x - mOldX);
                } else if (mChangeBrightness) {
                    onBrightnessSlide((mOldY - y) * 2 / currentHeight);
                } else if (mChangeVolume) {
                    onVolumeSlide(((mOldY - y) * 2 / currentHeight));
                }

                return true;

        }
        return false;
    }

    private void onSeekProgressControl(float seekDistance) {
        if (!target.isPlaying())
            return;
   //    Log.d("XPLAY","target.getDuration() =="+currentWidth+"dff  "+seekBar.getProgress()  );
        preDuration = seekBar.getProgress() + (int) ((seekDistance / currentWidth) * 30);
        if (preDuration > 100) {
            preDuration = 100;
        } else if (preDuration < 0) {
            preDuration = 0;
        }
        long time = preDuration * target.getDuration() / 100;
        if (llProgressTime.getVisibility() == View.GONE) {
            llProgressTime.setVisibility(View.VISIBLE);
        }
        isproChange=true;
        tvProgressTime.setText(Utils.stringForTime(time) + "/" + Utils.stringForTime(target.getDuration()));
    }

    /**
     * 滑动改变声音大小
     *
     * @param percent
     */
    private void onVolumeSlide(float percent) {
        pbOperation.setMax(mMaxVolume * 100);
        if (mVolume == -1) {
            if (mVolume < 0)
                mVolume = 0;
            mVolume = mAudioManager.getStreamVolume(AudioManager.STREAM_MUSIC);
            // 显示
            imgOperation.setImageResource(R.drawable.salient_volume);
            llOperation.setVisibility(View.VISIBLE);
        }
        float index = (percent * mMaxVolume) + mVolume;
        if (index > mMaxVolume)
            index = mMaxVolume;
        else if (index < 0)
            index = 0;

        // 变更声音
        mAudioManager.setStreamVolume(AudioManager.STREAM_MUSIC, (int) index, 0);

        // 变更进度条
        pbOperation.setProgress((int) (index * 100));
    }

    /**
     * 滑动改变亮度
     *
     * @param percent
     */
    private void onBrightnessSlide(float percent) {
        pbOperation.setMax((int) (1f * 100));
        if (mBrightness < 0) {
            mBrightness = ((Activity) mControlPanel.getContext()).getWindow().getAttributes().screenBrightness;
            if (mBrightness <= 0.00f)
                mBrightness = 0.50f;
            if (mBrightness < 0.01f)
                mBrightness = 0.01f;

            // 显示
            imgOperation.setImageResource(R.drawable.salient_brightness);
            llOperation.setVisibility(View.VISIBLE);
        }

        WindowManager.LayoutParams lpa = ((Activity) mControlPanel.getContext()).getWindow().getAttributes();
        lpa.screenBrightness = mBrightness + percent;
        if (lpa.screenBrightness > 1.0f) {
            lpa.screenBrightness = 1.0f;
        } else if (lpa.screenBrightness < 0.01f) {
            lpa.screenBrightness = 0.01f;
        }
        ((Activity) mControlPanel.getContext()).getWindow().setAttributes(lpa);

        // 变更进度条
        pbOperation.setProgress((int) (lpa.screenBrightness * 100));
    }


    private Runnable runnable = new Runnable() {
        @Override
        public void run() {
            llOperation.setVisibility(View.GONE);
            llProgressTime.setVisibility(View.GONE);
        }
    };

    @Override
    public boolean onTouch(View v, MotionEvent event) {

        final int action = event.getAction();
        if (action == MotionEvent.ACTION_UP) {
            //音量变量清空，延迟隐藏声音控件
            mVolume = -1;
            llOperation.postDelayed(runnable, 500);
            //亮度变量清空
            mBrightness = -1f;

            if (mChangePosition) {
                if (seekBar != null) {

                    Log.d("XPLAY","seekBar.setProgress(preDuration)");
                    if(isproChange) {
                        seekBar.setProgress(preDuration);
                        mControlPanel.onStopTrackingTouch(seekBar);
                        isproChange = false;
                    }
                }
            }
        }
        if (action == MotionEvent.ACTION_DOWN) {
            if(mControlPanel.isShowing()){
                mControlPanel.hideView();
            }
        }
        return false;
    }

    @Override
    public boolean onDoubleTap(MotionEvent e) {
//        if (mControlPanel.getTarget().isPlaying()) {
//            mControlPanel.getTarget().pause();
//        } else {
//            mControlPanel.getTarget().start();
//        }
        return true;
    }

}
