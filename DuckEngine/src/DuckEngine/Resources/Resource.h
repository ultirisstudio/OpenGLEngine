#pragma once

#include <string>

namespace DuckEngine
{
	class Resource
	{
	public:
		virtual ~Resource() {};

		virtual void load(const std::string& path) = 0;
		virtual void free() = 0;
	};
}