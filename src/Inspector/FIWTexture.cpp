#include "BangEditor/FIWTexture.h"

#include "Bang/Resources.h"
#include "Bang/Texture2D.h"

#include "Bang/UIComboBox.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

FIWTexture::FIWTexture()
{
    p_filterModeComboBox = GameObjectFactory::CreateUIComboBox();

    AddWidget("Filter Mode", p_filterModeComboBox->GetGameObject());

    SetLabelsWidth(60);
}

FIWTexture::~FIWTexture()
{
}

Texture2D *FIWTexture::GetTexture() const
{
    return p_texture.Get();
}

void FIWTexture::UpdateFromTextureFile()
{

}

void FIWTexture::OnPathChanged(const Path &path)
{
    if (path.IsFile())
    {
        p_texture = Resources::Load<Texture2D>(path);
    }
}

void FIWTexture::OnValueChanged(Object *object)
{

}

