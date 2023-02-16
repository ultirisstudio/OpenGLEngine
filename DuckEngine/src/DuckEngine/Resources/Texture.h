#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace DuckEngine
{
	class Texture
	{
	private:
		unsigned int m_id;
		std::string m_path;

	public:
		Texture();
		Texture(const std::string& path);
		~Texture();

		static std::shared_ptr<Texture> CreateTexture(const std::string& path);

		unsigned int GetID() const { return m_id; }
		std::string getPath() const;

		void bind() const;
	};
}