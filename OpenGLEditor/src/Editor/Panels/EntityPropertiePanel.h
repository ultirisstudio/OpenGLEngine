#pragma once

#include "../SceneManager.h"

#include "SceneHierarchy.h"

#include "ComponentPanels/TransformComponentPanel.h"
#include "ComponentPanels/ModelComponentPanel.h"
#include "ComponentPanels/CameraComponentPanel.h"
#include "ComponentPanels/MeshComponentPanel.h"
#include "ComponentPanels/TerrainComponentPanel.h"
#include "ComponentPanels/MaterialComponentPanel.h"
#include "ComponentPanels/LightComponentPanel.h"
#include "ComponentPanels/ScriptComponentPanel.h"
#include "ComponentPanels/RigidBodyComponentPanel.h"
#include "ComponentPanels/BoxColliderComponentPanel.h"
#include "ComponentPanels/MeshColliderComponentPanel.h"

namespace OpenGLEngine
{
	class EntityPropertiePanel
	{
	public:
		EntityPropertiePanel();

		void OnImGuiRender(SceneManager& sceneManager, SceneHierarchy& sceneHierarchy);
	private:
		std::unique_ptr<TransformComponentPanel> m_TransformComponentPanel;
		std::unique_ptr<ModelComponentPanel> m_ModelComponentPanel;
		std::unique_ptr<CameraComponentPanel> m_CameraComponentPanel;
		std::unique_ptr<MeshComponentPanel> m_MeshComponentPanel;
		std::unique_ptr<TerrainComponentPanel> m_TerrainComponentPanel;
		std::unique_ptr<MaterialComponentPanel> m_MaterialComponentPanel;
		std::unique_ptr<LightComponentPanel> m_LightComponentPanel;
		std::unique_ptr<ScriptComponentPanel> m_ScriptComponentPanel;
		std::unique_ptr<RigidBodyComponentPanel> m_RigidBodyComponentPanel;
		std::unique_ptr<BoxColliderComponentPanel> m_BoxColliderComponentPanel;
		std::unique_ptr<MeshColliderComponentPanel> m_MeshColliderComponentPanel;
	};
}