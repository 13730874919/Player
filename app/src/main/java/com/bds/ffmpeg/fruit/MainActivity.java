/*
* Copyright (C) 2015 Andy Ke <dictfb@gmail.com>
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/


package com.bds.ffmpeg.fruit;

import android.Manifest;
import android.app.Activity;
import android.content.ContentResolver;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.os.Bundle;
import android.provider.MediaStore;
import android.view.View;
import android.view.WindowManager;

import androidx.core.app.ActivityCompat;
import androidx.recyclerview.widget.GridLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.bds.ffmpeg.R;
import com.bds.ffmpeg.universalvideoview.UniversalActivity;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends Activity{
    private List<Fruit> fruitList = new ArrayList<>();
    private ContentResolver mContentResolver;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.fruit);
        mContentResolver= getContentResolver();
        checkPermission(this);

        getVideos();
        RecyclerView recyclerView = (RecyclerView) findViewById(R.id.recycler_view);

        RecyclerView.LayoutManager layoutManager = null;
        layoutManager = new GridLayoutManager(this, 2);
        recyclerView.setLayoutManager(layoutManager);
        FruitAdapter adapter = new FruitAdapter(fruitList,mContentResolver,DensityUtil.getWindowWidth(this));
        recyclerView.setAdapter(adapter);
        adapter.setOnItemClickListener(new FruitAdapter.OnItemClickListener() {
            @Override
            public void onItemClick(View view, int position) {

                Fruit item = fruitList.get(position);
                String path = item.getpath();
                Intent intent5 = new Intent(MainActivity.this, UniversalActivity.class);
                intent5.putExtra("path",path);
                startActivity(intent5);
            }
        });
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
    private  void getVideos() {


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
                Fruit item = new Fruit(name,id,path);
                fruitList.add(item);
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (c != null) {
                c.close();
            }
        }
    }
//    private void initFruits() {
//        for (int i = 0; i < 2; i++) {
//            Fruit apple = new Fruit("Apple", R.drawable.apple_pic);
//            fruitList.add(apple);
//            Fruit banana = new Fruit("Banana", R.drawable.banana_pic);
//            fruitList.add(banana);
//            Fruit orange = new Fruit("Orange", R.drawable.orange_pic);
//            fruitList.add(orange);
//            Fruit watermelon = new Fruit("Watermelon", R.drawable.watermelon_pic);
//            fruitList.add(watermelon);
//            Fruit pear = new Fruit("Pear", R.drawable.pear_pic);
//            fruitList.add(pear);
//            Fruit grape = new Fruit("Grape", R.drawable.grape_pic);
//            fruitList.add(grape);
//            Fruit pineapple = new Fruit("Pineapple", R.drawable.pineapple_pic);
//            fruitList.add(pineapple);
//            Fruit strawberry = new Fruit("Strawberry", R.drawable.strawberry_pic);
//            fruitList.add(strawberry);
//            Fruit cherry = new Fruit("Cherry", R.drawable.cherry_pic);
//            fruitList.add(cherry);
//            Fruit mango = new Fruit("Mango", R.drawable.mango_pic);
//            fruitList.add(mango);
//
//        }
//    }
}
