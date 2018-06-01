#include "BangEditor/FIWTextureCubeMap.h"

#include "Bang/UILabel.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"
#include "Bang/Extensions.h"
#include "Bang/UITextRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UIInputTexture.h"

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

    p_topTextureInput = GameObject::Create<UIInputTexture>();
    p_topTextureInput->SetExtensions( Extensions::GetImageExtensions() );
    p_topTextureInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    AddWidget("Top texture", p_topTextureInput);

    p_botTextureInput = GameObject::Create<UIInputTexture>();
    p_botTextureInput->SetExtensions( Extensions::GetImageExtensions() );
    p_botTextureInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    AddWidget("Bot texture", p_botTextureInput);

    p_leftTextureInput = GameObject::Create<UIInputTexture>();
    p_leftTextureInput->SetExtensions( Extensions::GetImageExtensions() );
    p_leftTextureInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    AddWidget("Left texture", p_leftTextureInput);

    p_rightTextureInput = GameObject::Create<UIInputTexture>();
    p_rightTextureInput->SetExtensions( Extensions::GetImageExtensions() );
    p_rightTextureInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    AddWidget("Right texture", p_rightTextureInput);

    p_frontTextureInput = GameObject::Create<UIInputTexture>();
    p_frontTextureInput->SetExtensions( Extensions::GetImageExtensions() );
    p_frontTextureInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    AddWidget("Front texture", p_frontTextureInput);

    p_backTextureInput = GameObject::Create<UIInputTexture>();
    p_backTextureInput->SetExtensions( Extensions::GetImageExtensions() );
    p_backTextureInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    AddWidget("Back texture", p_backTextureInput);

    p_warningLabel = GameObjectFactory::CreateUILabel();
    p_warningLabel->GetText()->SetContent("Please add all images so that cubemap "
                                          "is fully configured.");
    p_warningLabel->GetText()->SetWrapping(true);
    AddWidget(p_warningLabel->GetGameObject(), 60);

    SetLabelsWidth(100);
}

TextureCubeMap *FIWTextureCubeMap::GetTextureCubeMap() const
{
    return p_textureCubeMap.Get();
}

void FIWTextureCubeMap::CheckValidity() const
{
    TextureCubeMap *tcm = GetTextureCubeMap();
    RH<Imageb> topImg   = tcm->GetImageResource(GL::CubeMapDir::TOP);
    RH<Imageb> botImg   = tcm->GetImageResource(GL::CubeMapDir::BOT);
    RH<Imageb> leftImg  = tcm->GetImageResource(GL::CubeMapDir::LEFT);
    RH<Imageb> rightImg = tcm->GetImageResource(GL::CubeMapDir::RIGHT);
    RH<Imageb> frontImg = tcm->GetImageResource(GL::CubeMapDir::FRONT);
    RH<Imageb> backImg  = tcm->GetImageResource(GL::CubeMapDir::BACK);

    if (!topImg || !botImg || !leftImg || !rightImg || !frontImg || !backImg)
    {
        p_warningLabel->GetText()->SetTextColor(Color::Red);
        p_warningLabel->GetText()->SetContent("Please set all images so that cubemap "
                                              "is fully configured. Until then, "
                                              "it will not work.");
    }
    else
    {
        bool allSizesCorrect = true;
        Vector2i size = topImg.Get()->GetSize();
        allSizesCorrect = (size.x == size.y) &&
                          botImg.Get()->GetSize()   == size &&
                          leftImg.Get()->GetSize()  == size &&
                          rightImg.Get()->GetSize() == size &&
                          frontImg.Get()->GetSize() == size &&
                          backImg.Get()->GetSize()  == size;
        if (!allSizesCorrect)
        {
            p_warningLabel->GetText()->SetTextColor(Color::Red);
            p_warningLabel->GetText()->SetContent("All image sizes must be square "
                                                  "and all sides must have the same size.");
        }
        else
        {
            p_warningLabel->GetText()->SetTextColor(Color::Black);
            p_warningLabel->GetText()->SetContent("Cubemap correctly configured!");
        }
    }
}

void FIWTextureCubeMap::UpdateFromFileWhenChanged()
{
    p_textureCubeMap = Resources::Load<TextureCubeMap>( GetPath() );
    if (!GetTextureCubeMap()) { return; }

    EventListener<IEventsValueChanged>::SetReceiveEvents(false);

    TextureCubeMap *tcm = GetTextureCubeMap();
    const RH<Imageb> topImg   = tcm->GetImageResource(GL::CubeMapDir::TOP);
    const RH<Imageb> botImg   = tcm->GetImageResource(GL::CubeMapDir::BOT);
    const RH<Imageb> leftImg  = tcm->GetImageResource(GL::CubeMapDir::LEFT);
    const RH<Imageb> rightImg = tcm->GetImageResource(GL::CubeMapDir::RIGHT);
    const RH<Imageb> frontImg = tcm->GetImageResource(GL::CubeMapDir::FRONT);
    const RH<Imageb> backImg  = tcm->GetImageResource(GL::CubeMapDir::BACK);
    p_topTextureInput->SetPath(topImg     ? topImg.Get()->GetResourceFilepath()   : Path::Empty);
    p_botTextureInput->SetPath(botImg     ? botImg.Get()->GetResourceFilepath()   : Path::Empty);
    p_leftTextureInput->SetPath(leftImg   ? leftImg.Get()->GetResourceFilepath()  : Path::Empty);
    p_rightTextureInput->SetPath(rightImg ? rightImg.Get()->GetResourceFilepath() : Path::Empty);
    p_frontTextureInput->SetPath(frontImg ? frontImg.Get()->GetResourceFilepath() : Path::Empty);
    p_backTextureInput->SetPath(backImg   ? backImg.Get()->GetResourceFilepath()  : Path::Empty);

    CheckValidity();

    EventListener<IEventsValueChanged>::SetReceiveEvents(true);
}

void FIWTextureCubeMap::OnValueChanged(EventEmitter<IEventsValueChanged>*)
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

        Refresh(p_topTextureInput,   tcm, GL::CubeMapDir::TOP);
        Refresh(p_botTextureInput,   tcm, GL::CubeMapDir::BOT);
        Refresh(p_leftTextureInput,  tcm, GL::CubeMapDir::LEFT);
        Refresh(p_rightTextureInput, tcm, GL::CubeMapDir::RIGHT);
        Refresh(p_frontTextureInput, tcm, GL::CubeMapDir::FRONT);
        Refresh(p_backTextureInput,  tcm, GL::CubeMapDir::BACK);

        CheckValidity();

        const Path tcmImportPath = ImportFilesManager::GetImportFilepath(
                                                tcm->GetResourceFilepath());
        if (tcmImportPath.IsFile())
        {
            tcm->ExportXMLToFile(tcmImportPath);
        }
    }
}

