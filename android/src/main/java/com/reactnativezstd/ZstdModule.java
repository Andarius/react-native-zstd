package com.reactnativezstd;

import android.util.Log;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.JavaScriptContextHolder;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.module.annotations.ReactModule;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableNativeArray;

@ReactModule(name = ZstdModule.NAME)
public class ZstdModule extends ReactContextBaseJavaModule {
  public static final String NAME = "Zstd";
  public static final String CPP_LIB_NAME = "reactnativezstd";

  public ZstdModule(ReactApplicationContext reactContext) {
    super(reactContext);
  }

  @Override
  @NonNull
  public String getName() {
    return NAME;
  }

  static {
    try {
      Log.i(NAME, "Loading C++ library...");
      System.loadLibrary(CPP_LIB_NAME);
    } catch (Exception e) {
      Log.e(NAME, "Failed to load C++ library", e);
    }
  }


  private native void nativeInstall(long jsiPtr);

  @ReactMethod(isBlockingSynchronousMethod = true)
  public void install() {
    JavaScriptContextHolder jsContext = getReactApplicationContext().getJavaScriptContextHolder();
    this.nativeInstall(jsContext.get());
    Log.i(NAME, "Successfully installed ZSTD JSI bindings");
  }


//    // Example method
//    // See https://reactnative.dev/docs/native-modules-android
//    @ReactMethod
//    public void compress(String data, int compressionLevel, Promise promise) {
//
//      byte[] res = nativeCompress(data, compressionLevel, errorMsg);
//      WritableArray output = new WritableNativeArray();
//      for (byte re : res) {
//        output.pushInt(re);
//      }
//      promise.resolve(output, errorMsg);
//
//    }
//
//    public static native byte[] nativeCompress(String data, int compressionLevel);
}
