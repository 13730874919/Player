package com.bds.ffmpeg;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.SurfaceHolder;

public class XPlay extends GLSurfaceView implements SurfaceHolder.Callback {
    public XPlay(Context context, AttributeSet attrs) {
        super(context, attrs);
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
