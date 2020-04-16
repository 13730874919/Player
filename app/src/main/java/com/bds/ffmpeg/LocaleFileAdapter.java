package com.bds.ffmpeg;

import android.content.ContentResolver;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.provider.MediaStore;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.List;


/**
 * 本地文件adapter
 * 
 * @author zhanglei
 *
 */
public class LocaleFileAdapter extends BaseAdapter {
	private String TAG = "XPLAY";
	private List<Video> data;
	private Context cxt;
	private ContentResolver mContentResolver;

	public LocaleFileAdapter(List<Video> data, Context cxt ,ContentResolver c) {
		super();
		mContentResolver = c;
		this.data = data;
		this.cxt = cxt;
	}

	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		if(null!=data)
			return data.size();
		return 0;
	}

	@Override
	public Object getItem(int arg0) {
		// TODO Auto-generated method stub
		return arg0;
	}

	@Override
	public long getItemId(int arg0) {
		// TODO Auto-generated method stub
		return 0;
	}

	//目录：显示目录view;文件：显示文件view及勾选状况
	@Override
	public View getView(int pos, View view, ViewGroup arg2) {
		// TODO Auto-generated method stub
		if(null == view){
			view = LayoutInflater.from(cxt).inflate(R.layout.locale_video_item, null);
		}

		Video item = data.get(pos);
		TextView filename = (TextView) view.findViewById(R.id.filename);
		ImageView iv = (ImageView) view.findViewById(R.id.icon);
		iv.setImageBitmap(getVideoThumbnail(item.getid()));
		Log.d(TAG,"getName.siz==="+item.getName());
		filename.setText(item.getName());
		return view;
	}
	private Bitmap getVideoThumbnail(int id) {
		Bitmap bitmap = null;
		BitmapFactory.Options options = new BitmapFactory.Options();
		options.inDither = false;
		options.inPreferredConfig = Bitmap.Config.ARGB_8888;
		bitmap = MediaStore.Video.Thumbnails.getThumbnail(mContentResolver, id, MediaStore.Images.Thumbnails.MICRO_KIND, options);
		return bitmap;
	}

}
