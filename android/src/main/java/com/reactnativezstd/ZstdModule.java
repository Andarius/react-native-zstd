package com.reactnativezstd;

import androidx.annotation.NonNull;

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
            // Used to load the 'native-lib' library on application startup.
            System.loadLibrary("reactnativezstd");
        } catch (Exception ignored) {
        }
    }

    // Example method
    // See https://reactnative.dev/docs/native-modules-android
    @ReactMethod
    public void compress(String data, int compressionLevel, Promise promise) {
      byte[] res = nativeCompress(data, compressionLevel);
      WritableArray output = new WritableNativeArray();
      for (byte re : res) {
        output.pushInt(re);
      }
      promise.resolve(output);

    }

    public static native byte[] nativeCompress(String data, int compressionLevel);
}
