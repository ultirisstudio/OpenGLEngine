#pragma once

#include <string>

namespace QuasarEngine
{
	struct AssetHeader {
		uint32_t magic;
		uint32_t totalSize;
	};

	//struct AssetType {
	//	uint8_t name;
	//};

	struct TextureHeader {
		uint8_t format;
		uint8_t internal_format;

		uint8_t wrap_r;
		uint8_t wrap_s;
		uint8_t wrap_t;

		uint8_t min_filter_param;
		uint8_t mag_filter_param;

		uint32_t width;
		uint32_t height;

		uint8_t alpha;
		uint8_t gamma;
		uint8_t flip;
		uint8_t mipmap;
		uint8_t compressed;

		uint32_t Samples;
		uint32_t channels;
	};
}