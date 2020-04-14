package com.bds.ffmpeg;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.View;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class XPlay extends GLSurfaceView implements SurfaceHolder.Callback ,GLSurfaceView.Renderer, View.OnClickListener {
    public XPlay(Context context, AttributeSet attrs) {
        super(context, attrs);
        setOnClickListener( this );
        setRenderer(this);
    }

    @Override
    public void onClick(View v) {
        PlayOrPause();
    }
    //android 8.0 需要设置

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

    }
    public native void PlayOrPause();
    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {

    }

    @Override
    public void onDrawFrame(GL10 gl) {

    }

    public XPlay(Context context) {
        super(context);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
     //   super.surfaceCreated(holder);
        InitView(holder.getSurface());
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        super.surfaceDestroyed(holder);
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
     //   super.surfaceChanged(holder, format, w, h);
    }

    private native void InitView(Object obj);
}
