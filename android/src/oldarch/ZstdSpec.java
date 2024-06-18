package com.reactnativezstd;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.WritableArray;

abstract class ZstdSpec extends ReactContextBaseJavaModule {
  ZstdSpec(ReactApplicationContext context) {
    super(context);
  }
  public abstract WritableArray compress(String buffIn, double compressLevel);
  public abstract String decompress(ReadableArray buffIn);

}
