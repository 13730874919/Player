package com.bds.ffmpeg.fruit;

import android.content.ContentResolver;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.Drawable;
import android.media.MediaMetadataRetriever;
import android.provider.MediaStore;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.recyclerview.widget.RecyclerView;

import com.bds.ffmpeg.R;

import java.util.List;

public class FruitAdapter extends RecyclerView.Adapter<FruitAdapter.ViewHolder> {

    private List<Fruit> mFruitList;
    private ContentResolver mContentResolver;
    private OnItemClickListener mOnItemClickListener;


    public void setOnItemClickListener(OnItemClickListener mOnItemClickListener) {
        this.mOnItemClickListener = mOnItemClickListener;
    }
    public interface OnItemClickListener {
        void onItemClick(View view, int position);
    }
    static class ViewHolder extends RecyclerView.ViewHolder {
        ImageView fruitImage;
        TextView fruitName;

        public ViewHolder(View view) {
            super(view);
            fruitImage = (ImageView) view.findViewById(R.id.fruit_image);
            fruitName = (TextView) view.findViewById(R.id.fruitname);
        }

    }
    private final int mScreenWidth;
    private Context mContext;
    public FruitAdapter(List<Fruit> fruitList, ContentResolver c, int screenWidth, Context context) {
        mFruitList = fruitList;
        mContentResolver = c;
        mScreenWidth = screenWidth;
        mContext =context;
    }

    @Override

    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.fruit_item, parent, false);
        ViewHolder holder = new ViewHolder(view);
        return holder;
    }

    public void setdataList(List<Fruit> list){
        mFruitList=list;
    }
    @Override
    public void onBindViewHolder(final ViewHolder holder, int position) {

        final Fruit fruit = mFruitList.get(position);

        if(fruit.getlistSize()>1) {
            String fName = fruit.getpath().trim();
            String dir = fName.substring(fName.lastIndexOf("/")+1);
            holder.fruitName.setText(dir);
            Drawable drawable = mContext.getResources().getDrawable(R.drawable.folder);
            holder.fruitImage.setImageDrawable(drawable);
        }else {
            holder.fruitName.setText(fruit.getName());
            Bitmap bitmap = null;
            try {
                MediaMetadataRetriever media = new MediaMetadataRetriever();
                Log.d("XPLAY","file fruit.pathfilename()==  "+fruit.pathfilename());
                media.setDataSource(fruit.pathfilename());
                bitmap = media.getFrameAtTime();
                holder.fruitImage.setImageBitmap(bitmap);
            } catch (IllegalArgumentException e) {
                Drawable drawable = mContext.getResources().getDrawable(R.drawable.video);
                holder.fruitImage.setImageDrawable(drawable);
                e.printStackTrace();
            }

        }

        if(mOnItemClickListener != null){
            //为ItemView设置监听器
            holder.itemView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    int position = holder.getLayoutPosition(); // 1
                    mOnItemClickListener.onItemClick(holder.itemView,position);
                }
            });
        }


    }
    private Bitmap getVideoThumbnail(int id) {
        Bitmap bitmap = null;
        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inDither = false;
        options.inPreferredConfig = Bitmap.Config.ARGB_8888;
        bitmap = MediaStore.Video.Thumbnails.getThumbnail(mContentResolver, id, MediaStore.Images.Thumbnails.MICRO_KIND, options);
        return bitmap;
    }
    @Override
    public int getItemCount() {
        return mFruitList.size();
    }


}