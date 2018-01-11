#include "BangEditor/FIWTexture.h"

#include "Bang/UILabel.h"
#include "Bang/UISlider.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

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
    SetLabelsWidth(60);

    p_filterModeComboBox = GameObjectFactory::CreateUIComboBox();
    p_filterModeComboBox->EventEmitter<IValueChangedListener>::RegisterListener(this);
    p_filterModeComboBox->AddItem("Nearest",      int(GL::FilterMode::Nearest));
    p_filterModeComboBox->AddItem("Bilinear",     int(GL::FilterMode::Bilinear));
    p_filterModeComboBox->AddItem("Trilinear_NN", int(GL::FilterMode::Trilinear_NN));
    p_filterModeComboBox->AddItem("Trilinear_NL", int(GL::FilterMode::Trilinear_NL));
    p_filterModeComboBox->AddItem("Trilinear_LN", int(GL::FilterMode::Trilinear_LN));
    p_filterModeComboBox->AddItem("Trilinear_LL", int(GL::FilterMode::Trilinear_LL));

    p_wrapModeComboBox = GameObjectFactory::CreateUIComboBox();
    p_wrapModeComboBox->EventEmitter<IValueChangedListener>::RegisterListener(this);
    p_wrapModeComboBox->AddItem("Clamp", int(GL::WrapMode::ClampToEdge));
    p_wrapModeComboBox->AddItem("Repeat", int(GL::WrapMode::Repeat));

    p_alphaCutoffInput = GameObjectFactory::CreateUISlider();
    p_alphaCutoffInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    p_alphaCutoffInput->SetMinMaxValues(0.0f, 1.0f);

    p_textureImageRend = GameObjectFactory::CreateUIImage();

    AddWidget("Filter Mode", p_filterModeComboBox->GetGameObject());
    AddWidget("Wrap Mode", p_wrapModeComboBox->GetGameObject());
    AddWidget("Alpha Cutoff", p_alphaCutoffInput->GetGameObject());

    AddLabel("Texture");
    AddWidget(p_textureImageRend->GetGameObject(), 100);

    SetLabelsWidth(100);
}

Texture2D *FIWTexture::GetTexture() const
{
    return p_texture.Get();
}

void FIWTexture::UpdateFromTextureFile()
{
    Path texImportPath = ImportFilesManager::GetImportFilepath(
                                    GetTexture()->GetResourceFilepath() );
    GetTexture()->ImportXMLFromFile(texImportPath);

    IValueChangedListener::SetReceiveEvents(false);

    p_textureImageRend->SetImageTexture(p_texture.Get());
    p_textureImageRend->SetTint( p_texture.Get() ? Color::White : Color::Black );

    p_filterModeComboBox->SetSelectionByValue( int(GetTexture()->GetFilterMode()) );
    p_wrapModeComboBox->SetSelectionByValue( int(GetTexture()->GetWrapMode()) );
    p_alphaCutoffInput->SetValue( GetTexture()->GetAlphaCutoff() );

    IValueChangedListener::SetReceiveEvents(true);
}

void FIWTexture::OnPathChanged(const Path &path)
{
    if (path.IsFile())
    {
        p_texture = Resources::Load<Texture2D>(path);
        UpdateFromTextureFile();
    }
}

void FIWTexture::OnValueChanged(Object *object)
{
    if (GetTexture())
    {
        int filterMode = p_filterModeComboBox->GetSelectedValue();
        GetTexture()->SetFilterMode( SCAST<GL::FilterMode>(filterMode) );

        int wrapMode = p_wrapModeComboBox->GetSelectedValue();
        GetTexture()->SetWrapMode( SCAST<GL::WrapMode>(wrapMode) );

        GetTexture()->SetAlphaCutoff( p_alphaCutoffInput->GetValue() );

        Path texImportPath = ImportFilesManager::GetImportFilepath(
                                        GetTexture()->GetResourceFilepath() );
        if (texImportPath.IsFile())
        {
            GetTexture()->ExportXMLToFile(texImportPath);
        }
    }
}

