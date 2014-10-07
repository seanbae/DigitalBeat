package com.example.heartratemonitor;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.LinearLayout;

public class HalfSquareLayout extends LinearLayout{

    public HalfSquareLayout(Context context) {
        super(context);
    }



    public HalfSquareLayout(Context context, AttributeSet attrs) {
        super(context, attrs);
    }


    @Override
    public void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {

        super.onMeasure(widthMeasureSpec, widthMeasureSpec);
        int width = MeasureSpec.getSize(widthMeasureSpec) * 1/3;
        int height = MeasureSpec.getSize(heightMeasureSpec* 1/3);
        int size = width > height ? height : width;
        setMeasuredDimension(size, size);

    }
    
}
