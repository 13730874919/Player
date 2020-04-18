package com.bds.ffmpeg;

import android.Manifest;
import android.app.Activity;
import android.content.ContentResolver;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.os.Bundle;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;

import androidx.core.app.ActivityCompat;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class VideoListActivity extends Activity implements ListView.OnItemClickListener {


    private String TAG = "XPLAY";
    private ListView lv;

    private List<Video> datalist;
    private ContentResolver mContentResolver;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);



        setContentView(R.layout.video_list);
        checkPermission(this);
        mContentResolver= getContentResolver();
        lv = findViewById(R.id.listView);
    }
    private LocaleFileAdapter adapter;
    @Override
    protected void onResume() {
        super.onResume();
        Intent intent = getIntent();
        String action = intent.getAction();
        Log.d(TAG, "action==="+action);
        datalist = getVideos();
        Log.d(TAG,"datalist.siz==="+datalist.size());
        adapter = new LocaleFileAdapter(datalist,this,mContentResolver);
        lv.setAdapter(adapter);
        lv.setOnItemClickListener(this);
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
        Video item = datalist.get(position);
        String path = item.getPath();
        Log.d(TAG,"patch==="+path);
        Intent intent5 = new Intent(this, VideoViewActivity.class);
        intent5.putExtra("path",path);
        startActivity(intent5);

    }
    private  List<Video> getVideos() {

        List<Video> videos = new ArrayList<Video>();

        Cursor c = null;
        try {
            // String[] mediaColumns = { "_id", "_data", "_display_name",
            // "_size", "date_modified", "duration", "resolution" };

            c = mContentResolver.query(MediaStore.Video.Media.EXTERNAL_CONTENT_URI, null, null, null, MediaStore.Video.Media.DEFAULT_SORT_ORDER);
            while (c.moveToNext()) {
                String path = c.getString(c.getColumnIndexOrThrow(MediaStore.Video.Media.DATA));// 路径

               if (!fileIsExists(path)) {
                   continue;
               }

                int id = c.getInt(c.getColumnIndexOrThrow(MediaStore.Video.Media._ID));// 视频的id
                String name = c.getString(c.getColumnIndexOrThrow(MediaStore.Video.Media.DISPLAY_NAME)); // 视频名称
                String resolution = c.getString(c.getColumnIndexOrThrow(MediaStore.Video.Media.RESOLUTION)); //分辨率
                long size = c.getLong(c.getColumnIndexOrThrow(MediaStore.Video.Media.SIZE));// 大小
                long duration = c.getLong(c.getColumnIndexOrThrow(MediaStore.Video.Media.DURATION));// 时长
                long date = c.getLong(c.getColumnIndexOrThrow(MediaStore.Video.Media.DATE_MODIFIED));//修改时间

                Video video = new Video(id, path, name, resolution, size, date, duration);
                videos.add(video);
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (c != null) {
                c.close();
            }
        }
        return videos;
    }

    private void checkPermission(Activity activity) {
        // Storage Permissions
        final int REQUEST_EXTERNAL_STORAGE = 1;
        String[] PERMISSIONS_STORAGE = {
                Manifest.permission.READ_EXTERNAL_STORAGE,
                Manifest.permission.WRITE_EXTERNAL_STORAGE};

        try {
            //检测是否有写的权限
            int permission = ActivityCompat.checkSelfPermission(this,
                    "android.permission.WRITE_EXTERNAL_STORAGE");
            if (permission != PackageManager.PERMISSION_GRANTED) {
                // 没有写的权限，去申请写的权限，会弹出对话框
                ActivityCompat.requestPermissions(this, PERMISSIONS_STORAGE, REQUEST_EXTERNAL_STORAGE);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

    }
    public boolean fileIsExists(String strFile)
    {
        try
        {
            File f=new File(strFile);
            if(!f.exists())
            {
                return false;
            }

        }
        catch (Exception e)
        {
            return false;
        }

        return true;
    }

}
