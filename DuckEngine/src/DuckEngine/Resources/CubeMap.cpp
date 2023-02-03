#include "depch.h"
#include "CubeMap.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace DuckEngine
{
	CubeMap::CubeMap() : m_ID(0)
	{
		
	}
	void CubeMap::Load(std::vector<std::string> paths)
	{
		glGenTextures(1, &m_ID);

		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		for (unsigned int i = 0; i < paths.size(); i++)
		{
			stbi_set_flip_vertically_on_load(false);

			int width, height, nbChannels;
			unsigned char* data;

			data = stbi_load(paths[i].c_str(), &width, &height, &nbChannels, 0);

			if (data)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			else
				std::cout << "Failed to load cubemap texture at " << paths[i] << " : " << stbi_failure_reason() << std::endl;

			stbi_image_free(data);
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
	void CubeMap::Bind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
	}
}