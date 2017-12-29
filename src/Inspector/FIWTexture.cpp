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

    p_textureImageRend = GameObjectFactory::CreateUIImage();

    AddWidget("Filter Mode", p_filterModeComboBox->GetGameObject());
    AddWidget("Wrap Mode", p_wrapModeComboBox->GetGameObject());

    AddLabel("Texture");
    AddWidget(p_textureImageRend->GetGameObject(), 100);

    SetLabelsWidth(70);
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
    Path texImportPath = ImportFilesManager::GetImportFilePath(
                                    GetTexture()->GetResourceFilepath() );
    GetTexture()->ImportXMLFromFile(texImportPath);

    IValueChangedListener::SetReceiveEvents(false);

    p_filterModeComboBox->SetSelectionByValue( int(GetTexture()->GetFilterMode()) );
    p_wrapModeComboBox->SetSelectionByValue( int(GetTexture()->GetWrapMode()) );

    IValueChangedListener::SetReceiveEvents(true);
}

void FIWTexture::OnPathChanged(const Path &path)
{
    if (path.IsFile())
    {
        p_texture = Resources::Load<Texture2D>(path);
        p_textureImageRend->SetImageTexture(p_texture.Get());
        p_textureImageRend->SetTint( p_texture.Get() ? Color::White : Color::Black );
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

        Path texImportPath = ImportFilesManager::GetImportFilePath(
                                        GetTexture()->GetResourceFilepath() );
        Debug_Peek(texImportPath);
        if (texImportPath.IsFile())
        {
            GetTexture()->ExportXMLToFile(texImportPath);
        }
    }
}

