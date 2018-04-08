package com.example.zhangchi09.myapplication;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import org.chromium.net.CronetEngine;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {


    Button testButton;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        testButton = (Button) findViewById(R.id.testButton);
        testButton.setOnClickListener(this);
    }


    @Override
    public void onClick(View v) {
        if(v.getId() == R.id.testButton) {
            NetUtils.quicclient("10.101.41.110", 8300, "https://blog.it-cloud.cn:8300/demo/tiles");
        }
    }

}
