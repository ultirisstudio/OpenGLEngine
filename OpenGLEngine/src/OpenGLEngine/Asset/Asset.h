#pragma once

#include <OpenGLEngine/Core/UUID.h>

namespace OpenGLEngine
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