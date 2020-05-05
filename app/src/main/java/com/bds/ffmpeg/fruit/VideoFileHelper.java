package com.bds.ffmpeg.fruit;

import android.text.TextUtils;

import java.io.File;

public class VideoFileHelper {
    public static String getParent(String filePath) {
        if (TextUtils.isEmpty(filePath)) {
            return "";
        }
        File file = new File(filePath);
        return file.getParent();
    }

}
