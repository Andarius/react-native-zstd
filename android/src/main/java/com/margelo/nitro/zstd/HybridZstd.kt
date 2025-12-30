package com.margelo.nitro.zstd

import androidx.annotation.Keep
import com.facebook.proguard.annotations.DoNotStrip
import com.margelo.nitro.core.ArrayBuffer
import java.nio.ByteBuffer

@DoNotStrip
@Keep
class HybridZstd : HybridZstdSpec() {

    companion object {
        @JvmStatic
        private external fun nativeCompress(buffIn: String, compressLevel: Int): ByteArray

        @JvmStatic
        private external fun nativeDecompress(byteArray: ByteArray, sourceSize: Int): String

        init {
            System.loadLibrary("reactnativezstd")
        }
    }

    override fun compress(data: String, compressionLevel: Double): ArrayBuffer {
        val compressedBytes = nativeCompress(data, compressionLevel.toInt())
        // Allocate ArrayBuffer and copy data into it
        val arrayBuffer = ArrayBuffer.allocate(compressedBytes.size)
        val byteBuffer = arrayBuffer.getBuffer(false)
        byteBuffer.put(compressedBytes)
        byteBuffer.rewind()
        return arrayBuffer
    }

    override fun decompress(data: ArrayBuffer): String {
        val byteBuffer = data.getBuffer(false)
        val byteArray = ByteArray(data.size)
        byteBuffer.get(byteArray)
        return nativeDecompress(byteArray, byteArray.size)
    }
}
