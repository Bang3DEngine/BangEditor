#include "BangEditor/RIWTexture.h"

#include "Bang/AspectRatioMode.h"
#include "Bang/Color.h"
#include "Bang/EventEmitter.h"
#include "Bang/GL.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/MetaFilesManager.h"
#include "Bang/Path.h"
#include "Bang/RectTransform.h"
#include "Bang/ResourceHandle.h"
#include "Bang/Texture2D.h"
#include "Bang/UIAspectRatioFitter.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIInputText.h"
#include "Bang/UISlider.h"
#include "Bang/UITextRenderer.h"
#include "BangEditor/RIWResource.tcc"
#include "BangEditor/ResourceInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class IEventsValueChanged;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

RIWTexture::RIWTexture()
{
}

RIWTexture::~RIWTexture()
{
}

void RIWTexture::Init()
{
    ResourceInspectorWidget::Init();

    SetTitle("Texture");
    SetName("RIWTexture");

    p_filterModeComboBox = GameObjectFactory::CreateUIComboBox();
    p_filterModeComboBox->AddItem("Nearest",      int(GL::FilterMode::NEAREST));
    p_filterModeComboBox->AddItem("Bilinear",     int(GL::FilterMode::BILINEAR));
    p_filterModeComboBox->AddItem("Trilinear_NN", int(GL::FilterMode::TRILINEAR_NN));
    p_filterModeComboBox->AddItem("Trilinear_NL", int(GL::FilterMode::TRILINEAR_NL));
    p_filterModeComboBox->AddItem("Trilinear_LN", int(GL::FilterMode::TRILINEAR_LN));
    p_filterModeComboBox->AddItem("Trilinear_LL", int(GL::FilterMode::TRILINEAR_LL));
    p_filterModeComboBox->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_wrapModeComboBox = GameObjectFactory::CreateUIComboBox();
    p_wrapModeComboBox->AddItem("Clamp", int(GL::WrapMode::CLAMP_TO_EDGE));
    p_wrapModeComboBox->AddItem("Repeat", int(GL::WrapMode::REPEAT));
    p_wrapModeComboBox->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_alphaCutoffInput = GameObjectFactory::CreateUISlider();
    p_alphaCutoffInput->SetMinMaxValues(0.0f, 1.0f);
    p_alphaCutoffInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_SRGBCheckBoxInput = GameObjectFactory::CreateUICheckBox();
    p_SRGBCheckBoxInput->SetChecked(true);
    p_SRGBCheckBoxInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_textureWidth = GameObjectFactory::CreateUIInputText();
    p_textureWidth->SetBlocked(true);

    p_textureHeight = GameObjectFactory::CreateUIInputText();
    p_textureHeight->SetBlocked(true);

    GameObject *imageContainerGo = GameObjectFactory::CreateUIGameObject();

    GameObject *imageGo = GameObjectFactory::CreateUIGameObject();
    imageGo->GetRectTransform()->SetAnchorX( Vector2(0, 0) );
    imageGo->GetRectTransform()->SetAnchorY( Vector2(1, 1) );
    imageGo->GetRectTransform()->SetPivotPosition( Vector2(0, 1) );
    p_imageAspectRatioFitter = imageGo->AddComponent<UIAspectRatioFitter>();
    p_imageAspectRatioFitter->SetAspectRatioMode(AspectRatioMode::KEEP);
    p_textureImageRend = imageGo->AddComponent<UIImageRenderer>();
    GameObjectFactory::AddOuterBorder(imageGo);
    imageGo->SetParent(imageContainerGo);

    AddWidget(imageGo, 200);
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Width",  p_textureWidth->GetGameObject());
    AddWidget("Height", p_textureHeight->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Filter Mode", p_filterModeComboBox->GetGameObject());
    AddWidget("Wrap Mode", p_wrapModeComboBox->GetGameObject());
    AddWidget("Alpha Cutoff", p_alphaCutoffInput->GetGameObject());
    AddWidget("SRGB", p_SRGBCheckBoxInput->GetGameObject());

    SetLabelsWidth(100);
}

Texture2D *RIWTexture::GetTexture() const
{
    return SCAST<Texture2D*>(GetResource().Get());
}

void RIWTexture::UpdateInputsFromResource()
{
    p_textureImageRend->SetImageTexture( GetTexture() );
    p_textureImageRend->SetTint(Color::White);
    p_imageAspectRatioFitter->SetAspectRatio( GetTexture()->GetSize() );
    p_imageAspectRatioFitter->Invalidate();

    p_SRGBCheckBoxInput->SetChecked(
                GetTexture()->GetFormat() == GL::ColorFormat::SRGB ||
                GetTexture()->GetFormat() == GL::ColorFormat::SRGBA );
    p_filterModeComboBox->SetSelectionByValue( int(GetTexture()->GetFilterMode()) );
    p_wrapModeComboBox->SetSelectionByValue( int(GetTexture()->GetWrapMode()) );
    p_alphaCutoffInput->SetValue( GetTexture()->GetAlphaCutoff() );

    p_textureWidth->GetText()->SetContent(
                String::ToString(GetTexture()->GetWidth()) + " px");
    p_textureHeight->GetText()->SetContent(
                String::ToString(GetTexture()->GetHeight()) + " px");
}

Texture2D *RIWTexture::GetIconTexture() const
{
    return GetTexture();
}

void RIWTexture::OnValueChangedRIWResource(EventEmitter<IEventsValueChanged>*)
{
    Path texImportPath = MetaFilesManager::GetMetaFilepath(
                                    GetTexture()->GetResourceFilepath());

    int filterMode = p_filterModeComboBox->GetSelectedValue();
    GetTexture()->SetFilterMode( SCAST<GL::FilterMode>(filterMode) );

    int wrapMode = p_wrapModeComboBox->GetSelectedValue();
    GetTexture()->SetWrapMode( SCAST<GL::WrapMode>(wrapMode) );

    GetTexture()->SetAlphaCutoff( p_alphaCutoffInput->GetValue() );

    GL::ColorFormat newColorFormat = p_SRGBCheckBoxInput->IsChecked() ?
                                                  GL::ColorFormat::SRGBA :
                                                  GL::ColorFormat::RGBA8;
    GetTexture()->SetFormat(newColorFormat);
}

