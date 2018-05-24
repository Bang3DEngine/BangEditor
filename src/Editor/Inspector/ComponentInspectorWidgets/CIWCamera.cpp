#include "BangEditor/CIWCamera.h"

#include "Bang/Light.h"
#include "Bang/Camera.h"
#include "Bang/UISlider.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/TextureCubeMap.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UIInputFile.h"
#include "BangEditor/UIInputColor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void CIWCamera::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWCamera");
    SetTitle("Camera");

    p_zNearInput = GameObjectFactory::CreateUIInputNumber();
    p_zNearInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("ZNear", p_zNearInput->GetGameObject());

    p_zFarInput = GameObjectFactory::CreateUIInputNumber();
    p_zFarInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("ZFar", p_zFarInput->GetGameObject());

    p_orthoHeightInput = GameObjectFactory::CreateUIInputNumber();
    p_orthoHeightInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    p_orthoHeightInput->SetMinMaxValues(0.0f, Math::Infinity<float>());
    AddWidget("Ortho Height", p_orthoHeightInput->GetGameObject());

    p_fovInput = GameObjectFactory::CreateUISlider();
    p_fovInput->SetMinMaxValues(0.1f, 180.0f);
    p_fovInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("FOV", p_fovInput->GetGameObject());

    p_projectionModeInput = GameObjectFactory::CreateUIComboBox();
    p_projectionModeInput->AddItem("Orthographic",
                                   SCAST<int>(Camera::ProjectionMode::ORTHOGRAPHIC));
    p_projectionModeInput->AddItem("Perspective",
                                   SCAST<int>(Camera::ProjectionMode::PERSPECTIVE));
    p_projectionModeInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Projection Mode", p_projectionModeInput->GetGameObject());

    p_clearModeInput = GameObjectFactory::CreateUIComboBox();
    p_clearModeInput->AddItem("Color",  SCAST<int>(Camera::ClearMode::COLOR));
    p_clearModeInput->AddItem("SkyBox", SCAST<int>(Camera::ClearMode::SKY_BOX));
    p_clearModeInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Clear Mode", p_clearModeInput->GetGameObject());

    p_clearColorInput = GameObject::Create<UIInputColor>();
    p_clearColorInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Clear Color", p_clearColorInput);

    p_textureCubeMapInput = GameObject::Create<UIInputFile>();
    p_textureCubeMapInput->SetExtensions( {Extensions::GetTextureCubeMapExtension()} );
    p_textureCubeMapInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("SkyBox", p_textureCubeMapInput);

    SetLabelsWidth(90);
}

void CIWCamera::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    if (!p_zNearInput->HasFocus())
    {
        p_zNearInput->SetValue( GetCamera()->GetZNear() );
    }

    if (!p_zFarInput->HasFocus())
    {
        p_zFarInput->SetValue( GetCamera()->GetZFar() );
    }

    if (!p_orthoHeightInput->HasFocus())
    {
        p_orthoHeightInput->SetValue( GetCamera()->GetOrthoHeight() );
    }

    if (!p_fovInput->HasFocus())
    {
        p_fovInput->SetValue( GetCamera()->GetFovDegrees() );
    }

    if (!p_projectionModeInput->HasFocus())
    {
        p_projectionModeInput->SetSelectionByValue(
                    SCAST<int>(GetCamera()->GetProjectionMode()) );
    }

    if (!p_clearModeInput->HasFocus())
    {
        p_clearModeInput->SetSelectionByValue(
                    SCAST<int>(GetCamera()->GetClearMode()) );
    }

    if (!p_clearColorInput->HasFocus())
    {
        p_clearColorInput->SetColor( GetCamera()->GetClearColor() );
    }

    TextureCubeMap *skyBoxTex = GetCamera()->GetSkyBoxTexture();
    if (skyBoxTex)
    {
        p_textureCubeMapInput->SetPath(skyBoxTex->GetResourceFilepath());
    }

    LimitValues();
}

Camera *CIWCamera::GetCamera() const
{
    return SCAST<Camera*>( GetComponent() );
}

void CIWCamera::LimitValues()
{
    p_zNearInput->SetMinMaxValues(0.1f, GetCamera()->GetZFar());
    p_zFarInput->SetMinMaxValues(GetCamera()->GetZNear(), Math::Infinity<float>());
}

void CIWCamera::OnValueChanged(Object *object)
{
    ComponentInspectorWidget::OnValueChanged(object);

    GetCamera()->SetZNear( p_zNearInput->GetValue() );
    GetCamera()->SetZFar( p_zFarInput->GetValue() );
    GetCamera()->SetOrthoHeight( p_orthoHeightInput->GetValue() );
    GetCamera()->SetFovDegrees( p_fovInput->GetValue() );
    GetCamera()->SetProjectionMode(SCAST<Camera::ProjectionMode>(
                                     p_projectionModeInput->GetSelectedValue()) );
    GetCamera()->SetClearMode( SCAST<Camera::ClearMode>(
                                  p_clearModeInput->GetSelectedValue()) );
    GetCamera()->SetClearColor( p_clearColorInput->GetColor() );

    RH<TextureCubeMap> tcmRH;
    if (p_textureCubeMapInput->GetPath().IsFile())
    {
        tcmRH = Resources::Load<TextureCubeMap>(
                                    p_textureCubeMapInput->GetPath()).Get();
    }
    GetCamera()->SetSkyBoxTexture(tcmRH.Get());

    LimitValues();
}

