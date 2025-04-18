package com.reactnativezstd;

import com.facebook.react.bridge.Arguments
import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.bridge.ReadableArray
import com.facebook.react.bridge.WritableArray
import com.facebook.react.module.annotations.ReactModule


@ReactModule(name = ZstdModule.NAME)
class ZstdModule(reactContext: ReactApplicationContext) :
  NativeZstdSpec(reactContext) {

  override fun getName(): String {
    return NAME
  }

  companion object {
    const val NAME = "Zstd"

    @JvmStatic
    private external fun nativeCompress(buffIn: String, compressLevel: Int): ByteArray

    @JvmStatic
    private external fun nativeDecompress(byteArray: ByteArray, sourceSize: Int): String

    // Load the native library once when the class is loaded
    init {
      System.loadLibrary("reactnativezstd")
    }
  }

override fun compress(buffIn: String, compressLevel: Double): WritableArray {
    val compressedData = nativeCompress(buffIn, compressLevel.toInt())
    return compressedData.toWritableArray()
}


override fun decompress(buffIn: ReadableArray): String {
    val byteArray = ByteArray(buffIn.size())
    for (i in 0 until buffIn.size()) {
        byteArray[i] = buffIn.getInt(i).toByte()
    }
    return nativeDecompress(byteArray, byteArray.size)
}


private fun ByteArray.toWritableArray(): WritableArray {
      return Arguments.createArray().apply {
          forEach { pushInt(it.toInt()) }
      }
}

}
