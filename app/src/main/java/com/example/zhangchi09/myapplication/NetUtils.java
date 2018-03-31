package com.example.zhangchi09.myapplication;

public class NetUtils {
    static {
        System.loadLibrary("neteasenet");
    }
    private static native void nativeSendRequest(String url);
    public static void sendRequest(String url) {
        nativeSendRequest(url);
    }
}