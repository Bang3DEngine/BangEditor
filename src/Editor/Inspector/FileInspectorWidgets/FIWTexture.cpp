#include "BangEditor/FIWTexture.h"

#include "Bang/UILabel.h"
#include "Bang/UISlider.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"
#include "Bang/UIComboBox.h"
#include "Bang/UICheckBox.h"
#include "Bang/RectTransform.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutIgnorer.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIAspectRatioFitter.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

FIWTexture::FIWTexture()
{
}

FIWTexture::~FIWTexture()
{
}

void FIWTexture::Init()
{
    FileInspectorWidget::Init();

    SetTitle("Texture");
    SetName("FIWTexture");

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

    GameObject *imageContainerGo = GameObjectFactory::CreateUIGameObject();

    GameObject *imageGo = GameObjectFactory::CreateUIGameObject();
    imageGo->GetRectTransform()->SetAnchorX( Vector2(0, 0) );
    imageGo->GetRectTransform()->SetAnchorY( Vector2(1, 1) );
    imageGo->GetRectTransform()->SetPivotPosition( Vector2(0, 1) );
    p_imageAspectRatioFitter = imageGo->AddComponent<UIAspectRatioFitter>();
    p_imageAspectRatioFitter->SetAspectRatioMode(AspectRatioMode::KEEP);
    p_textureImageRend = imageGo->AddComponent<UIImageRenderer>();
    imageGo->SetParent(imageContainerGo);

    AddWidget("Filter Mode", p_filterModeComboBox->GetGameObject());
    AddWidget("Wrap Mode", p_wrapModeComboBox->GetGameObject());
    AddWidget("Alpha Cutoff", p_alphaCutoffInput->GetGameObject());
    AddWidget("SRGB", p_SRGBCheckBoxInput->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddLabel("Texture");
    AddWidget(imageGo, 400);

    SetLabelsWidth(100);
}

Texture2D *FIWTexture::GetTexture() const
{
    return p_texture.Get();
}

void FIWTexture::UpdateFromFileWhenChanged()
{
    p_texture = Resources::Load<Texture2D>( GetPath() );
    if (!GetTexture()) { return; }

    EventListener<IEventsValueChanged>::SetReceiveEvents(false);

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

    EventListener<IEventsValueChanged>::SetReceiveEvents(true);
}

void FIWTexture::OnValueChanged(EventEmitter<IEventsValueChanged>*)
{
    if (GetTexture())
    {
        int filterMode = p_filterModeComboBox->GetSelectedValue();
        GetTexture()->SetFilterMode( SCAST<GL::FilterMode>(filterMode) );

        int wrapMode = p_wrapModeComboBox->GetSelectedValue();
        GetTexture()->SetWrapMode( SCAST<GL::WrapMode>(wrapMode) );

        GetTexture()->SetAlphaCutoff( p_alphaCutoffInput->GetValue() );

        GL::ColorFormat newColorFormat = p_SRGBCheckBoxInput->IsChecked() ?
                                                GL::ColorFormat::SRGBA :
                                                GL::ColorFormat::RGBA8;
        GetTexture()->SetFormat(newColorFormat);

        Path texImportPath = ImportFilesManager::GetImportFilepath(
                                GetTexture()->GetResourceFilepath());
        if (texImportPath.IsFile())
        {
            GetTexture()->ExportXMLToFile(texImportPath);
        }
    }
}

