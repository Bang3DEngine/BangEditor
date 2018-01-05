#include "BangEditor/FIWMaterial.h"

#include "Bang/Material.h"
#include "Bang/UISlider.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"
#include "Bang/Extensions.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UIInputVector.h"
#include "BangEditor/UIInputFile.h"
#include "BangEditor/UIInputColor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

FIWMaterial::FIWMaterial()
{
}

FIWMaterial::~FIWMaterial()
{
}

void FIWMaterial::Init()
{
    FileInspectorWidget::Init();

    SetName("FIWMaterial");
    SetTitle("Material");

    p_texturePathInput = GameObject::Create<UIInputFile>();
    p_texturePathInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    p_texturePathInput->SetExtensions( Extensions::GetImageExtensions() );

    p_uvMultiplyInput = GameObject::Create<UIInputVector>();
    p_uvMultiplyInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    p_uvMultiplyInput->SetSize(2);

    p_diffuseColorInput = GameObject::Create<UIInputColor>();
    p_diffuseColorInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    UILayoutElement *diffColorLE = p_diffuseColorInput->AddComponent<UILayoutElement>();
    diffColorLE->SetFlexibleWidth(1.0f);

    p_receivesLightingCheckBox = GameObjectFactory::CreateUICheckBox();
    p_receivesLightingCheckBox->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_shininessSlider = GameObjectFactory::CreateUISlider();
    p_shininessSlider->EventEmitter<IValueChangedListener>::RegisterListener(this);
    p_shininessSlider->SetMinMaxValues(0.0f, 60.0f);

    AddWidget("Diff Color",  p_diffuseColorInput);
    AddWidget("Rec. light",  p_receivesLightingCheckBox->GetGameObject());
    AddWidget("Shininess",   p_shininessSlider->GetGameObject());
    AddWidget("Texture",     p_texturePathInput);
    AddWidget("Uv Multiply", p_uvMultiplyInput);

    SetLabelsWidth(75);
}

void FIWMaterial::UpdateFromMaterialFile()
{
    GetMaterial()->ImportXMLFromFile( GetMaterial()->GetResourceFilepath() );

    IValueChangedListener::SetReceiveEvents(false);

    Texture2D *matTex = GetMaterial()->GetTexture();
    p_texturePathInput->SetPath( matTex ? matTex->GetResourceFilepath() : Path::Empty);

    p_uvMultiplyInput->Set( GetMaterial()->GetUvMultiply() );
    p_diffuseColorInput->SetColor( GetMaterial()->GetDiffuseColor() );
    p_receivesLightingCheckBox->SetChecked( GetMaterial()->GetReceivesLighting() );
    p_shininessSlider->SetValue( GetMaterial()->GetShininess() );

    IValueChangedListener::SetReceiveEvents(true);
}

Material *FIWMaterial::GetMaterial() const
{
    return p_material.Get();
}

void FIWMaterial::OnPathChanged(const Path &path)
{
    p_material = Resources::Load<Material>(path);
    UpdateFromMaterialFile();
}

void FIWMaterial::OnValueChanged(Object *object)
{
    Path texPath = p_texturePathInput->GetPath();
    if (texPath.IsFile())
    {
        RH<Texture2D> tex = Resources::Load<Texture2D>(texPath);
        GetMaterial()->SetTexture(tex.Get());
    }
    else { GetMaterial()->SetTexture(nullptr); }

    GetMaterial()->SetUvMultiply( p_uvMultiplyInput->GetVector2() );
    GetMaterial()->SetDiffuseColor(p_diffuseColorInput->GetColor());
    GetMaterial()->SetReceivesLighting(p_receivesLightingCheckBox->IsChecked());
    GetMaterial()->SetShininess(p_shininessSlider->GetValue());

    Path matPath = GetMaterial()->GetResourceFilepath();
    if (matPath.IsFile())
    {
        GetMaterial()->ExportXMLToFile(matPath);
    }
}
