package com.bds.ffmpeg.fruit;

import java.util.ArrayList;

public class Fruit {

    private String name;
    private String path;
    public ArrayList<String> filelist = new ArrayList<String>();
    public Fruit(String name, String path){
        this.name = name;
        this.path = path;
    }
    public String getName() {
        return name;
    }
    public String getpath() {
        return path;
    }
    public String pathfilename() {
        return path+"/"+name;
    }
    public int getlistSize() {
        return filelist.size();
    }
}
