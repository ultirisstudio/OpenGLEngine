#pragma once

#include "DuckEngine/Resources/CubeMap.h"
#include "DuckEngine/Shader/Shader.h"
#include "DuckEngine/Core/Core.h"
#include "DuckEngine/Renderer/RenderModel.h"

namespace DuckEngine
{
	class Skybox
	{
	public:
		Skybox(Model& model);
		void Draw();
	private:
		CubeMap m_CubeMap;
		Shader m_CubeMapShader;
		std::shared_ptr<RenderModel> m_CubeMapModel;
	};
}