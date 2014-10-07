package com.example.heartratemonitor;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.LinearLayout;

public class SquareLayout extends LinearLayout{

    public SquareLayout(Context context) {
        super(context);
    }



    public SquareLayout(Context context, AttributeSet attrs) {
        super(context, attrs);
    }


    @Override
    public void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {

        super.onMeasure(widthMeasureSpec, widthMeasureSpec);
        int width = MeasureSpec.getSize(widthMeasureSpec) * 2/3;
        int height = MeasureSpec.getSize(heightMeasureSpec)* 2/3;
        int size = width > height ? height : width;
        setMeasuredDimension(size, size);

    }
    
}
