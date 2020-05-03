package com.bds.ffmpeg.fruit;
public class Fruit {

    private String name;
    private String path;
    private int imageId;

    public Fruit(String name, int imageId, String path){
        this.name = name;
        this.imageId = imageId;
        this.path = path;
    }

    public String getName() {
        return name;
    }
    public String getpath() {
        return path;
    }

    public int getImageId() {
        return imageId;
    }
}
