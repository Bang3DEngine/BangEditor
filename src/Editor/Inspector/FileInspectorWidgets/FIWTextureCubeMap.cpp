#include "BangEditor/FIWTextureCubeMap.h"

#include "Bang/Resources.h"
#include "Bang/Texture2D.h"
#include "Bang/Extensions.h"

#include "BangEditor/UIInputFile.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

FIWTextureCubeMap::FIWTextureCubeMap()
{
}

FIWTextureCubeMap::~FIWTextureCubeMap()
{
}

void FIWTextureCubeMap::Init()
{
    FileInspectorWidget::Init();

    SetTitle("Texture Cube Map");
    SetName("FIWTextureCubeMap");

    p_topTextureInput = GameObject::Create<UIInputFile>();
    p_topTextureInput->SetExtensions( Extensions::GetImageExtensions() );
    p_topTextureInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Top texture", p_topTextureInput);

    p_botTextureInput = GameObject::Create<UIInputFile>();
    p_botTextureInput->SetExtensions( Extensions::GetImageExtensions() );
    p_botTextureInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Bot texture", p_botTextureInput);

    p_leftTextureInput = GameObject::Create<UIInputFile>();
    p_leftTextureInput->SetExtensions( Extensions::GetImageExtensions() );
    p_leftTextureInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Left texture", p_leftTextureInput);

    p_rightTextureInput = GameObject::Create<UIInputFile>();
    p_rightTextureInput->SetExtensions( Extensions::GetImageExtensions() );
    p_rightTextureInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Right texture", p_rightTextureInput);

    p_frontTextureInput = GameObject::Create<UIInputFile>();
    p_frontTextureInput->SetExtensions( Extensions::GetImageExtensions() );
    p_frontTextureInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Front texture", p_frontTextureInput);

    p_backTextureInput = GameObject::Create<UIInputFile>();
    p_backTextureInput->SetExtensions( Extensions::GetImageExtensions() );
    p_backTextureInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Back texture", p_backTextureInput);

    SetLabelsWidth(100);
}

TextureCubeMap *FIWTextureCubeMap::GetTextureCubeMap() const
{
    return p_textureCubeMap.Get();
}

void FIWTextureCubeMap::UpdateFromFileWhenChanged()
{
    p_textureCubeMap = Resources::Load<TextureCubeMap>( GetPath() );
    if (!GetTextureCubeMap()) { return; }

    IValueChangedListener::SetReceiveEvents(false);

    TextureCubeMap *tcm = GetTextureCubeMap();
    const RH<Imageb> topImg   = tcm->GetImageResource(GL::CubeMapDir::Top);
    const RH<Imageb> botImg   = tcm->GetImageResource(GL::CubeMapDir::Bot);
    const RH<Imageb> leftImg  = tcm->GetImageResource(GL::CubeMapDir::Left);
    const RH<Imageb> rightImg = tcm->GetImageResource(GL::CubeMapDir::Right);
    const RH<Imageb> frontImg = tcm->GetImageResource(GL::CubeMapDir::Front);
    const RH<Imageb> backImg  = tcm->GetImageResource(GL::CubeMapDir::Back);
    p_topTextureInput->SetPath(topImg     ? topImg.Get()->GetResourceFilepath()   : Path::Empty);
    p_botTextureInput->SetPath(botImg     ? botImg.Get()->GetResourceFilepath()   : Path::Empty);
    p_leftTextureInput->SetPath(leftImg   ? leftImg.Get()->GetResourceFilepath()  : Path::Empty);
    p_rightTextureInput->SetPath(rightImg ? rightImg.Get()->GetResourceFilepath() : Path::Empty);
    p_frontTextureInput->SetPath(frontImg ? frontImg.Get()->GetResourceFilepath() : Path::Empty);
    p_backTextureInput->SetPath(backImg   ? backImg.Get()->GetResourceFilepath()  : Path::Empty);

    IValueChangedListener::SetReceiveEvents(true);
}

void FIWTextureCubeMap::OnValueChanged(Object *object)
{
    TextureCubeMap *tcm = GetTextureCubeMap();
    if (tcm)
    {
        auto Refresh = [this](UIInputFile *inputFile,
                              TextureCubeMap *tcm,
                              GL::CubeMapDir cmdir)
        {
            if (inputFile->GetPath().IsFile())
            {
                RH<Imageb> img = Resources::Load<Imageb>( inputFile->GetPath() );
                tcm->SetImageResource(cmdir, img.Get());
            }
            else { tcm->SetImageResource(cmdir, nullptr); }

        };

        Refresh(p_topTextureInput,   tcm, GL::CubeMapDir::Top);
        Refresh(p_botTextureInput,   tcm, GL::CubeMapDir::Bot);
        Refresh(p_leftTextureInput,  tcm, GL::CubeMapDir::Left);
        Refresh(p_rightTextureInput, tcm, GL::CubeMapDir::Right);
        Refresh(p_frontTextureInput, tcm, GL::CubeMapDir::Front);
        Refresh(p_backTextureInput,  tcm, GL::CubeMapDir::Back);

        const Path tcmImportPath = ImportFilesManager::GetImportFilepath(
                                                tcm->GetResourceFilepath());
        if (tcmImportPath.IsFile())
        {
            tcm->ExportXMLToFile(tcmImportPath);
        }
    }
}

