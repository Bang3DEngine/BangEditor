#include "BangEditor/FIWMaterial.h"

#include "Bang/Material.h"
#include "Bang/Resources.h"
#include "Bang/UICheckBox.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

FIWMaterial::FIWMaterial()
{
    SetName("FIWMaterial");
    SetTitle("Material");

    p_receivesLightingCheckBox = GameObjectFactory::CreateUICheckBox();
    AddWidget("Receives lighting", p_receivesLightingCheckBox->GetGameObject());

    SetLabelsWidth(100);
}

FIWMaterial::~FIWMaterial()
{
}

void FIWMaterial::OnPathChanged(const Path &path)
{
    p_material = Resources::Load<Material>(path);
}
