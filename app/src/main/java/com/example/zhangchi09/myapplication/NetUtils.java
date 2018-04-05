package com.example.zhangchi09.myapplication;

public class NetUtils {
    static {
        System.loadLibrary("neteasenet");
    }

    private static native void nativeQuicClient(String host, int port, String path);


    public static void quicclient(String host, int port, String path) {
        nativeQuicClient(host, port, path);
    }

}