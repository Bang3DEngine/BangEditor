#include "BangEditor/CIWReflectionProbe.h"

#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/Extensions.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIComboBox.h"
#include "Bang/RectTransform.h"
#include "Bang/UIInputNumber.h"
#include "Bang/ReflectionProbe.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UIInputColor.h"
#include "BangEditor/UIInputVector.h"
#include "BangEditor/UIInputFileWithPreview.h"
#include "BangEditor/UITextureCubeMapPreviewer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

CIWReflectionProbe::CIWReflectionProbe()
{
}

CIWReflectionProbe::~CIWReflectionProbe()
{
}

void CIWReflectionProbe::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWReflectionProbe");
    SetTitle("Reflection Probe");

    p_isBoxedCheckBox = GameObjectFactory::CreateUICheckBox();
    p_isBoxedCheckBox->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_filterForIBLCheckBox = GameObjectFactory::CreateUICheckBox();
    p_filterForIBLCheckBox->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_sizeInput = GameObject::Create<UIInputVector>();
    p_sizeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_sizeInput->SetMinValue(Vector4::Zero);
    p_sizeInput->SetSize(3);

    p_restTimeInput = GameObjectFactory::CreateUIInputNumber();
    p_restTimeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_restTimeInput->SetMinValue(0.0f);
    p_restTimeInput->SetDecimalPlaces(1);

    p_reflectionProbSizeInput = GameObjectFactory::CreateUIComboBox();
    p_reflectionProbSizeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_reflectionProbSizeInput->AddItem("128",  128);
    p_reflectionProbSizeInput->AddItem("256",  256);
    p_reflectionProbSizeInput->AddItem("512",  512);
    p_reflectionProbSizeInput->AddItem("1024", 1024);
    p_reflectionProbSizeInput->AddItem("2048", 2048);

    p_zNearInput = GameObjectFactory::CreateUIInputNumber();
    p_zNearInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_zFarInput = GameObjectFactory::CreateUIInputNumber();
    p_zFarInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_clearModeInput = GameObjectFactory::CreateUIComboBox();
    p_clearModeInput->AddItem("Color",  SCAST<int>(CameraClearMode::COLOR));
    p_clearModeInput->AddItem("SkyBox", SCAST<int>(CameraClearMode::SKY_BOX));
    p_clearModeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_clearColorInput = GameObject::Create<UIInputColor>();
    p_clearColorInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_textureCubeMapInput = GameObject::Create<UIInputFileWithPreview>();
    p_textureCubeMapInput->SetExtensions( {Extensions::GetTextureCubeMapExtension()} );
    p_textureCubeMapInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_previewCMRenderer = GameObject::Create<UITextureCubeMapPreviewer>();

    AddWidget("Render size", p_reflectionProbSizeInput->GetGameObject());
    AddWidget("Rest time sec.", p_restTimeInput->GetGameObject());
    AddWidget("Boxed", p_isBoxedCheckBox->GetGameObject());
    AddWidget("Filter for IBL", p_filterForIBLCheckBox->GetGameObject());
    AddWidget("Size",  p_sizeInput);
    AddWidget( GameObjectFactory::CreateUIHSeparator(), 10 );
    AddWidget("ZNear", p_zNearInput->GetGameObject());
    AddWidget("ZFar", p_zFarInput->GetGameObject());
    AddWidget("Clear Mode", p_clearModeInput->GetGameObject());
    AddWidget("Clear Color", p_clearColorInput);
    AddWidget("SkyBox", p_textureCubeMapInput);
    AddWidget( GameObjectFactory::CreateUIHSeparator(), 10 );
    AddLabel("Preview");
    AddWidget(p_previewCMRenderer, 200);

    SetLabelsWidth(100);
}

void CIWReflectionProbe::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    ReflectionProbe *reflProbe = GetReflectionProbe();

    if (!p_zNearInput->HasFocus())
    {
        p_zNearInput->SetValue( reflProbe->GetCamerasZNear() );
    }

    if (!p_zFarInput->HasFocus())
    {
        p_zFarInput->SetValue( reflProbe->GetCamerasZFar() );
    }

    if (!p_clearModeInput->HasFocus())
    {
        p_clearModeInput->SetSelectionByValue(
                    SCAST<int>(reflProbe->GetCamerasClearMode()) );
    }

    if (!p_clearColorInput->HasFocus())
    {
        p_clearColorInput->SetColor( reflProbe->GetCamerasClearColor() );
    }

    if (!p_reflectionProbSizeInput->HasFocus())
    {
        p_reflectionProbSizeInput->SetSelectionByValue( reflProbe->GetRenderSize() );
    }

    TextureCubeMap *skyBoxTex = reflProbe->GetCamerasSkyBoxTexture();
    if (skyBoxTex)
    {
        p_textureCubeMapInput->SetPath(skyBoxTex->GetResourceFilepath());
    }

    if (!p_sizeInput->HasFocus())
    {
        p_sizeInput->Set( reflProbe->GetSize() );
    }

    if (!p_restTimeInput->HasFocus())
    {
        p_restTimeInput->SetValue( reflProbe->GetRestTime().GetSeconds() );
    }

    p_isBoxedCheckBox->SetChecked( reflProbe->GetIsBoxed() );
    p_filterForIBLCheckBox->SetChecked( reflProbe->GetFilterForIBL() );
    p_previewCMRenderer->SetTextureCubeMap( reflProbe->
                                            GetTextureCubeMapWithoutFiltering() );

    p_previewCMRenderer->InvalidateRenderer();
}

void CIWReflectionProbe::LimitValues()
{
    p_zNearInput->SetMinMaxValues(0.1f, GetReflectionProbe()->GetCamerasZFar());
    p_zFarInput->SetMinValue(GetReflectionProbe()->GetCamerasZNear());
}

void CIWReflectionProbe::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

    ReflectionProbe *reflProbe = GetReflectionProbe();
    reflProbe->SetCamerasZNear( p_zNearInput->GetValue() );
    reflProbe->SetCamerasZFar( p_zFarInput->GetValue() );
    reflProbe->SetCamerasClearMode( SCAST<CameraClearMode>(
                                    p_clearModeInput->GetSelectedValue()) );
    reflProbe->SetCamerasClearColor( p_clearColorInput->GetColor() );
    reflProbe->SetRenderSize( p_reflectionProbSizeInput->GetSelectedValue() );

    RH<TextureCubeMap> tcmRH;
    if (p_textureCubeMapInput->GetPath().IsFile())
    {
        tcmRH = Resources::Load<TextureCubeMap>(
                                    p_textureCubeMapInput->GetPath());
    }
    reflProbe->SetCamerasSkyBoxTexture(tcmRH.Get());

    reflProbe->SetSize( p_sizeInput->GetVector3() );
    reflProbe->SetIsBoxed( p_isBoxedCheckBox->IsChecked() );
    reflProbe->SetRestTimeSeconds( p_restTimeInput->GetValue() );
    reflProbe->SetFilterForIBL( p_filterForIBLCheckBox->IsChecked() );

    p_previewCMRenderer->InvalidateRenderer();
}

ReflectionProbe *CIWReflectionProbe::GetReflectionProbe() const
{
    return SCAST<ReflectionProbe*>( GetComponent() );
}
