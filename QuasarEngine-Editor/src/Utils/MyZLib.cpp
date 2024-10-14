#include "MyZlib.h"

#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <zlib.h>

namespace QuasarEngine
{
    std::vector<char> MyZLib::compress(const std::vector<char>& data) {
        z_stream zs;
        memset(&zs, 0, sizeof(zs));

        if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK) {
            throw(std::runtime_error("deflateInit failed while compressing."));
        }

        zs.next_in = (Bytef*)data.data();
        zs.avail_in = data.size();

        int ret;
        char outbuffer[10240];
        std::vector<char> compressedData;

        do {
            zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
            zs.avail_out = sizeof(outbuffer);

            ret = deflate(&zs, Z_FINISH);

            if (compressedData.size() < zs.total_out) {
                compressedData.insert(compressedData.end(), outbuffer, outbuffer + zs.total_out - compressedData.size());
            }
        } while (ret == Z_OK);

        deflateEnd(&zs);

        if (ret != Z_STREAM_END) {
            std::ostringstream oss;
            oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
            throw(std::runtime_error(oss.str()));
        }

        return compressedData;
    }

    std::vector<char> MyZLib::decompress(const std::vector<char>& data) {
        z_stream zs;
        memset(&zs, 0, sizeof(zs));

        if (inflateInit(&zs) != Z_OK) {
            throw(std::runtime_error("inflateInit failed while decompressing."));
        }

        zs.next_in = (Bytef*)data.data();
        zs.avail_in = data.size();

        int ret;
        char outbuffer[10240];
        std::vector<char> decompressedData;

        do {
            zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
            zs.avail_out = sizeof(outbuffer);

            ret = inflate(&zs, 0);

            if (decompressedData.size() < zs.total_out) {
                decompressedData.insert(decompressedData.end(), outbuffer, outbuffer + zs.total_out - decompressedData.size());
            }

        } while (ret == Z_OK);

        inflateEnd(&zs);

        if (ret != Z_STREAM_END) {
            std::ostringstream oss;
            oss << "Exception during zlib decompression: (" << ret << ") " << zs.msg;
            throw(std::runtime_error(oss.str()));
        }

        return decompressedData;
    }
}