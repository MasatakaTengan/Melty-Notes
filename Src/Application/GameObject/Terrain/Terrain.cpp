#include "Terrain.h"

void Terrain::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData("Asset/Data/Sample/Terrain/StageMap.gltf");
	}
}

void Terrain::Update()
{
}

void Terrain::DrawLit()
{
	if (!m_spModel) return;

	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel);
}

