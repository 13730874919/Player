package com.bds.ffmpeg;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.SeekBar;

import androidx.core.app.ActivityCompat;

public class MainActivity extends Activity implements Runnable{

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

    private Button bt;
    private SeekBar seek;
    private Thread th;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);//隐藏标题
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);//设置全屏

        setContentView(R.layout.activity_main);
        checkPermission(this);
        seek = findViewById( R.id.aplayseek );
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
        bt = findViewById( R.id.open_button );
        bt.setOnClickListener( new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.e("XPlay","open button click!");
                //打开选择路径窗口
                Intent intent = new Intent();
                intent.setClass( MainActivity.this ,OpenUrl.class);
                startActivity( intent );


            }
        } );
        //启动播放进度线程
        th = new Thread(this);
        th.start();
    }

            private void checkPermission(Activity activity) {
                // Storage Permissions
                final int REQUEST_EXTERNAL_STORAGE = 1;
                String[] PERMISSIONS_STORAGE = {
                        Manifest.permission.READ_EXTERNAL_STORAGE,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE};

                try {
                    //检测是否有写的权限
                    int permission = ActivityCompat.checkSelfPermission(MainActivity.this,
                            "android.permission.WRITE_EXTERNAL_STORAGE");
                    if (permission != PackageManager.PERMISSION_GRANTED) {
                        // 没有写的权限，去申请写的权限，会弹出对话框
                        ActivityCompat.requestPermissions(MainActivity.this, PERMISSIONS_STORAGE, REQUEST_EXTERNAL_STORAGE);
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }

            }

    public native double PlayPos();
    public native void Seek(double pos);
}
