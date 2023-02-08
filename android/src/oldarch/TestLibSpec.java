package com.reactnativezstd;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.Promise;

abstract class ZstdSpec extends ReactContextBaseJavaModule {
  ZstdSpec(ReactApplicationContext context) {
    super(context);
  }

  public abstract void install();
}
