#pragma once

struct GLFWwindow;

namespace QuasarEngine
{
	class VulkanRenderer
	{
	public:
		struct VulkanData
		{
			
		};
		static VulkanData m_VulkanData;

		static void Init(GLFWwindow* window);
	};
}