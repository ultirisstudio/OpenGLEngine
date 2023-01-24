#pragma once

#include "DuckEngine.h"

namespace DuckEngine
{
	class Editor : public Layer
	{
	public:
		Editor();
		virtual ~Editor() = default;

		void OnUpdate() override;
		void OnImGuiRender() override;
	};
}