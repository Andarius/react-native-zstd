import Foundation
import NitroModules

class HybridZstd: HybridZstdSpec {
    func compress(data: String, compressionLevel: Double) throws -> ArrayBuffer {
        var error: NSError?
        guard let compressedData = ZstdBridge.compressString(data, compressionLevel: Int32(compressionLevel), error: &error) else {
            throw error ?? NSError(domain: "ZstdError", code: -1, userInfo: [NSLocalizedDescriptionKey: "Compression failed"])
        }

        return try ArrayBuffer.copy(data: compressedData)
    }

    func decompress(data: ArrayBuffer) throws -> String {
        let nsData = Data(bytes: data.data, count: data.size)

        var error: NSError?
        guard let decompressedString = ZstdBridge.decompress(nsData, error: &error) else {
            throw error ?? NSError(domain: "ZstdError", code: -1, userInfo: [NSLocalizedDescriptionKey: "Decompression failed"])
        }

        return decompressedString
    }
}
