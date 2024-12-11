#pragma once

#include "VulkanTypes.h"

namespace QuasarEngine {

	class VulkanDevice
	{
	public:
		static vulkanDevice m_VulkanDevice;

		static bool CreateDevice();
		static void DestroyDevice();
	};
}