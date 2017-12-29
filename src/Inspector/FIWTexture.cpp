#include "BangEditor/FIWTexture.h"

#include "Bang/UILabel.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"
#include "Bang/UIComboBox.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

FIWTexture::FIWTexture()
{
    SetTitle("Texture");
    SetName("FIWTexture");
    SetLabelsWidth(60);

    p_filterModeComboBox = GameObjectFactory::CreateUIComboBox();
    p_filterModeComboBox->AddItem("Hola", 0);
    p_filterModeComboBox->AddItem("Que", 1);
    // p_filterModeComboBox->AddItem("Tal", 2);
    // p_filterModeComboBox->AddItem("Estamos", 3);

    p_textureImageRend = GameObjectFactory::CreateUIImage();

    AddWidget("Filter Mode", p_filterModeComboBox->GetGameObject());

    AddLabel("Texture");
    AddWidget(p_textureImageRend->GetGameObject(), 100);
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
        p_textureImageRend->SetImageTexture(p_texture.Get());
        p_textureImageRend->SetTint( p_texture.Get() ? Color::White : Color::Black );
    }
}

void FIWTexture::OnValueChanged(Object *object)
{

}

