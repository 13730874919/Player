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
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.text.TextUtils;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

import androidx.core.app.ActivityCompat;
import androidx.recyclerview.widget.GridLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.bds.ffmpeg.R;
import com.bds.ffmpeg.universalvideoview.UniversalActivity;

import java.io.File;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends Activity{
    private List<Fruit> fruitList = new ArrayList<>();
    private List<Fruit> CurrenttList = new ArrayList<>();
    private ContentResolver mContentResolver;

    private int currentDir=1;
    private   FruitAdapter adapter;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        checkPermission(this);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.fruit);
        mContentResolver= getContentResolver();
        getVideos();
        RecyclerView recyclerView = (RecyclerView) findViewById(R.id.recycler_view);

        RecyclerView.LayoutManager layoutManager = null;
        layoutManager = new GridLayoutManager(this, 2);
        recyclerView.setLayoutManager(layoutManager);
        adapter = new FruitAdapter(fruitList,mContentResolver,DensityUtil.getWindowWidth(this),this);
        recyclerView.setAdapter(adapter);
        CurrenttList=fruitList;
        currentDir=1;
        adapter.setOnItemClickListener(new FruitAdapter.OnItemClickListener() {
            @Override
            public void onItemClick(View view, int position) {

                Fruit item = CurrenttList.get(position);
                if(item.getlistSize()>1){
                    ArrayList<Fruit> mFruitList = new ArrayList<Fruit>();
                    for (int i=0; i<item.getlistSize();i++){
                        String name = item.filelist.get(i);
                        Fruit newitem = new Fruit(name,item.getpath());
                        newitem.filelist.add(name);
                        mFruitList.add(newitem);
                    }
                    CurrenttList=mFruitList;
                    currentDir=2;
                    adapter.setdataList(mFruitList);
                    adapter.notifyDataSetChanged();
                }else {
                    String path = item.pathfilename();
                    Intent intent5 = new Intent(MainActivity.this, UniversalActivity.class);
                    intent5.putExtra("path", path);
                    startActivity(intent5);
                }
            }
        });
        int [] ii=getNotchSize(this);
        Log.d("XPLAY","file name==  "+ii[0]);
        Log.d("XPLAY","file name==  "+ii[1]);
    }

    @Override
    protected void onResume() {
        super.onResume();
        Intent intent = getIntent();
        Uri _url = intent.getData();

//        Intent it = new Intent(MainActivity.this, UniversalActivity.class);
//        it.putExtra("path", "http://vfx.mtime.cn/Video/2018/07/06/mp4/180706094003288023.mp4");
//        startActivity(it);


        if(_url!=null) {
            String path = getFilePathFromContentUri(_url,mContentResolver);
            Log.d("XPLAY", "getScheme===" +  path);
            Intent intent5 = new Intent(MainActivity.this, UniversalActivity.class);
            intent5.putExtra("path", path);
            startActivity(intent5);
        }
    }
    public static String getFilePathFromContentUri(Uri selectedVideoUri,
                                                   ContentResolver contentResolver) {
        String filePath;
        String[] filePathColumn = {MediaStore.MediaColumns.DATA};

        Cursor cursor = contentResolver.query(selectedVideoUri, filePathColumn, null, null, null);
//      也可用下面的方法拿到cursor
//      Cursor cursor = this.context.managedQuery(selectedVideoUri, filePathColumn, null, null, null);

        cursor.moveToFirst();

        int columnIndex = cursor.getColumnIndex(filePathColumn[0]);
        filePath = cursor.getString(columnIndex);
        cursor.close();
        return filePath;
    }

    /*刘海屏全屏显示FLAG*/
    public static final int FLAG_NOTCH_SUPPORT=0x00010000;
    /**
     * 设置应用窗口在华为刘海屏手机使用刘海区
     * @param window 应用页面window对象
     */
    public static void setFullScreenWindowLayoutInDisplayCutout(Window window) {
        if (window == null) {
            return;
        }
        WindowManager.LayoutParams layoutParams = window.getAttributes();
        try {
            Class layoutParamsExCls = Class.forName("com.huawei.android.view.LayoutParamsEx");
            Constructor con=layoutParamsExCls.getConstructor(WindowManager.LayoutParams.class);
            Object layoutParamsExObj=con.newInstance(layoutParams);
            Method method=layoutParamsExCls.getMethod("addHwFlags", int.class);
            method.invoke(layoutParamsExObj, FLAG_NOTCH_SUPPORT);
        } catch (ClassNotFoundException | NoSuchMethodException | IllegalAccessException |InstantiationException
                | InvocationTargetException e) {
            Log.e("test", "hw add notch screen flag api error");
        } catch (Exception e) {
            Log.e("test", "other Exception");
        }
    }

    public static int[] getNotchSize(Context context) {
        int[] ret = new int[]{0, 0};
        try {
            ClassLoader cl = context.getClassLoader();
            Class HwNotchSizeUtil = cl.loadClass("com.huawei.android.util.HwNotchSizeUtil");
            Method get = HwNotchSizeUtil.getMethod("getNotchSize");
            ret = (int[]) get.invoke(HwNotchSizeUtil);
        } catch (ClassNotFoundException e) {
            Log.e("test", "getNotchSize ClassNotFoundException");
        } catch (NoSuchMethodException e) {
            Log.e("test", "getNotchSize NoSuchMethodException");
        } catch (Exception e) {
            Log.e("test", "getNotchSize Exception");
        } finally {
            return ret;
        }
    }
    public static boolean hasNotchInScreen(Context context) {
        boolean ret = false;
        try {
            ClassLoader cl = context.getClassLoader();
            Class HwNotchSizeUtil = cl.loadClass("com.huawei.android.util.HwNotchSizeUtil");
            Method get = HwNotchSizeUtil.getMethod("hasNotchInScreen");
            ret = (boolean) get.invoke(HwNotchSizeUtil);
        } catch (ClassNotFoundException e) {
            Log.e("test", "hasNotchInScreen ClassNotFoundException");
        } catch (NoSuchMethodException e) {
            Log.e("test", "hasNotchInScreen NoSuchMethodException");
        } catch (Exception e) {
            Log.e("test", "hasNotchInScreen Exception");
        } finally {
            return ret;
        }
    }
    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        if(event.getKeyCode()==KeyEvent.KEYCODE_BACK){
            if(currentDir==2){
                CurrenttList=fruitList;
                currentDir=1;
                adapter.setdataList(CurrenttList);
                adapter.notifyDataSetChanged();
                return true;
            }else {
                finish();
            }
        }
        return super.onKeyUp(keyCode, event);
    }

    private void checkPermission(Activity activity) {
        // Storage Permissions
        final int REQUEST_EXTERNAL_STORAGE = 1;
        String[] PERMISSIONS_STORAGE = {
                Manifest.permission.READ_EXTERNAL_STORAGE,
                Manifest.permission.WRITE_EXTERNAL_STORAGE,
                Manifest.permission.INTERNET
        };

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
                String parent = VideoFileHelper.getParent(path);
                int id = c.getInt(c.getColumnIndexOrThrow(MediaStore.Video.Media._ID));// 视频的id
                String name = c.getString(c.getColumnIndexOrThrow(MediaStore.Video.Media.DISPLAY_NAME)); // 视频名称
                Fruit item = new Fruit(name,parent);
                if(!TextUtils.isEmpty(parent)){
                    Boolean quit = false;
                    for (int i = 0; i < fruitList.size(); i++) {
                            if (fruitList.get(i).getpath().equals(parent)) {
                                quit=true;
                                fruitList.get(i).filelist.add(name);
                                break;
                            }
                    }
                    if(quit){
                        continue;}
                }
                item.filelist.add(name);
                fruitList.add(item);
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (c != null) {
                c.close();
            }
        }
        Log.d("XPLAY","file fruitList.size()=="+fruitList.size());
        for(int i=0; i< fruitList.size(); i++){
            Log.d("XPLAY","file path==  "+fruitList.get(i).getpath());
            for(int j=0; j<fruitList.get(i).filelist.size();j++){
                String name = fruitList.get(i).filelist.get(j);
                Log.d("XPLAY","file name==  "+name);
            }
        }
    }

}
