package com.reactnativezstd;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.module.annotations.ReactModule;

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
            System.loadLibrary("cpp");
        } catch (Exception ignored) {
        }
    }

    // Example method
    // See https://reactnative.dev/docs/native-modules-android
    @ReactMethod
    public void multiply(double a, double b, Promise promise) {
        promise.resolve(nativeMultiply(a, b));
    }

    public static native double nativeMultiply(double a, double b);
}
