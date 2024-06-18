package com.reactnativezstd;

import android.util.Log;


import androidx.annotation.NonNull;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactApplicationContext;


import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.ReactMethod;


public class ZstdModule extends ZstdSpec {
  public static final String NAME = "Zstd";
  public static final String CPP_LIB_NAME = "reactnativezstd";

  public ZstdModule(ReactApplicationContext context) {
    super(context);
  }

  @Override
  @NonNull
  public String getName() {
    return NAME;
  }

  static {
    Log.i(NAME, "Loading C++ library...");
    try {
      System.loadLibrary(CPP_LIB_NAME);
    } catch (Exception e) {
      Log.e(NAME, "Failed to load C++ library", e);
    }
  }


  public static native byte[] nativeCompress(String buffIn,
                                             int compressLevel);

  public static native String nativeDecompress(byte[] buffIn, int sourceSize);


  @ReactMethod(isBlockingSynchronousMethod = true)
  public WritableArray compress(String buffIn, double compressLevel) {
    byte[] compressedData = nativeCompress(buffIn, (int)compressLevel);
    WritableArray compressedDataArray = Arguments.createArray();
    for (byte b : compressedData) {
        compressedDataArray.pushInt(b);
    }
    return compressedDataArray;
  }

  @ReactMethod(isBlockingSynchronousMethod = true)
  public String decompress(ReadableArray buffIn) {
     byte[] byteArray = new byte[buffIn.size()];
     for (int i = 0; i < buffIn.size(); i++) {
          byteArray[i] = (byte) buffIn.getInt(i);
      }
      return nativeDecompress(byteArray, byteArray.length);
  }


}
