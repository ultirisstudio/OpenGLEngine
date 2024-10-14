#pragma once

#include <QuasarEngine/Core/UUID.h>

namespace QuasarEngine
{
	enum class AssetType
	{
		NONE = 0,
		TEXTURE,
		MODEL,
		MESH
	};

	class Asset
	{
	public:
		UUID ID;

		virtual AssetType GetType() = 0;
	};
}