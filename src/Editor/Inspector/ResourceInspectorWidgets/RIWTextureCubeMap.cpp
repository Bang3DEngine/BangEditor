#include "BangEditor/RIWTextureCubeMap.h"

#include "Bang/Color.h"
#include "Bang/EventEmitter.h"
#include "Bang/Extensions.h"
#include "Bang/GL.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/Image.h"
#include "Bang/ImageIO.h"
#include "Bang/Path.h"
#include "Bang/ResourceHandle.h"
#include "Bang/Texture2D.h"
#include "Bang/UILabel.h"
#include "Bang/UITextRenderer.h"
#include "BangEditor/RIWResource.tcc"
#include "BangEditor/ResourceInspectorWidget.h"
#include "BangEditor/UIInputFile.h"
#include "BangEditor/UIInputTexture.h"
#include "BangEditor/UITextureCubeMapPreviewer.h"

namespace Bang
{
class IEventsValueChanged;
}

using namespace Bang;
using namespace BangEditor;

RIWTextureCubeMap::RIWTextureCubeMap()
{
}

RIWTextureCubeMap::~RIWTextureCubeMap()
{
}

void RIWTextureCubeMap::Init()
{
    ResourceInspectorWidget::Init();

    SetTitle("Texture Cube Map");
    SetName("RIWTextureCubeMap");

    p_topTextureInput = new UIInputTexture();
    p_topTextureInput->SetExtensions(Extensions::GetImageExtensions());
    p_topTextureInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    AddWidget("Top texture", p_topTextureInput);

    p_botTextureInput = new UIInputTexture();
    p_botTextureInput->SetExtensions(Extensions::GetImageExtensions());
    p_botTextureInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    AddWidget("Bot texture", p_botTextureInput);

    p_leftTextureInput = new UIInputTexture();
    p_leftTextureInput->SetExtensions(Extensions::GetImageExtensions());
    p_leftTextureInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    AddWidget("Left texture", p_leftTextureInput);

    p_rightTextureInput = new UIInputTexture();
    p_rightTextureInput->SetExtensions(Extensions::GetImageExtensions());
    p_rightTextureInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    AddWidget("Right texture", p_rightTextureInput);

    p_frontTextureInput = new UIInputTexture();
    p_frontTextureInput->SetExtensions(Extensions::GetImageExtensions());
    p_frontTextureInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    AddWidget("Front texture", p_frontTextureInput);

    p_backTextureInput = new UIInputTexture();
    p_backTextureInput->SetExtensions(Extensions::GetImageExtensions());
    p_backTextureInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    AddWidget("Back texture", p_backTextureInput);

    p_warningLabel = GameObjectFactory::CreateUILabel();
    p_warningLabel->GetText()->SetContent(
        "Please add all images so that cubemap "
        "is fully configured.");
    p_warningLabel->GetText()->SetWrapping(true);
    AddWidget(p_warningLabel->GetGameObject(), 60);

    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);

    p_textureCMPreviewer = new UITextureCubeMapPreviewer();
    p_textureCMPreviewer->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    AddLabel("Preview");
    AddWidget(p_textureCMPreviewer, 256);

    SetLabelsWidth(100);
}

TextureCubeMap *RIWTextureCubeMap::GetTextureCubeMap() const
{
    return SCAST<TextureCubeMap *>(GetResource().Get());
}

void RIWTextureCubeMap::CheckValidity() const
{
    TextureCubeMap *tcm = GetTextureCubeMap();
    RH<Texture2D> topTex = tcm->GetSideTexture(GL::CubeMapDir::TOP);
    RH<Texture2D> botTex = tcm->GetSideTexture(GL::CubeMapDir::BOT);
    RH<Texture2D> leftTex = tcm->GetSideTexture(GL::CubeMapDir::LEFT);
    RH<Texture2D> rightTex = tcm->GetSideTexture(GL::CubeMapDir::RIGHT);
    RH<Texture2D> frontTex = tcm->GetSideTexture(GL::CubeMapDir::FRONT);
    RH<Texture2D> backTex = tcm->GetSideTexture(GL::CubeMapDir::BACK);

    if (!topTex || !botTex || !leftTex || !rightTex || !frontTex || !backTex)
    {
        p_warningLabel->GetText()->SetTextColor(Color::Red());
        p_warningLabel->GetText()->SetContent(
            "Please set all images so that cubemap "
            "is fully configured. Until then, "
            "it will not work.");
    }
    else
    {
        bool allSizesCorrect = true;
        Vector2i size = topTex.Get()->GetSize();
        allSizesCorrect = (size.x == size.y) &&
                          botTex.Get()->GetSize() == size &&
                          leftTex.Get()->GetSize() == size &&
                          rightTex.Get()->GetSize() == size &&
                          frontTex.Get()->GetSize() == size &&
                          backTex.Get()->GetSize() == size;
        if (!allSizesCorrect)
        {
            p_warningLabel->GetText()->SetTextColor(Color::Red());
            p_warningLabel->GetText()->SetContent(
                "All image sizes must be square "
                "and all sides must have the same size.");
        }
        else
        {
            p_warningLabel->GetText()->SetTextColor(Color::Black());
            p_warningLabel->GetText()->SetContent(
                "Cubemap correctly configured!");
        }
    }
}

void RIWTextureCubeMap::UpdateInputsFromResource()
{
    TextureCubeMap *tcm = GetTextureCubeMap();
    const RH<Texture2D> topTex = tcm->GetSideTexture(GL::CubeMapDir::TOP);
    const RH<Texture2D> botTex = tcm->GetSideTexture(GL::CubeMapDir::BOT);
    const RH<Texture2D> leftTex = tcm->GetSideTexture(GL::CubeMapDir::LEFT);
    const RH<Texture2D> rightTex = tcm->GetSideTexture(GL::CubeMapDir::RIGHT);
    const RH<Texture2D> frontTex = tcm->GetSideTexture(GL::CubeMapDir::FRONT);
    const RH<Texture2D> backTex = tcm->GetSideTexture(GL::CubeMapDir::BACK);
    p_topTextureInput->SetPath(topTex ? topTex.Get()->GetResourceFilepath()
                                      : Path::Empty);
    p_botTextureInput->SetPath(botTex ? botTex.Get()->GetResourceFilepath()
                                      : Path::Empty);
    p_leftTextureInput->SetPath(leftTex ? leftTex.Get()->GetResourceFilepath()
                                        : Path::Empty);
    p_rightTextureInput->SetPath(
        rightTex ? rightTex.Get()->GetResourceFilepath() : Path::Empty);
    p_frontTextureInput->SetPath(
        frontTex ? frontTex.Get()->GetResourceFilepath() : Path::Empty);
    p_backTextureInput->SetPath(backTex ? backTex.Get()->GetResourceFilepath()
                                        : Path::Empty);

    p_textureCMPreviewer->SetTextureCubeMap(tcm);

    CheckValidity();
}

Texture2D *RIWTextureCubeMap::GetIconTexture() const
{
    return GetTextureCubeMap()
               ? GetTextureCubeMap()
                     ->GetSideTexture(GL::CubeMapDir::FRONT)
                     .Get()
               : nullptr;
}

void RIWTextureCubeMap::OnValueChangedRIWResource(
    EventEmitter<IEventsValueChanged> *ee)
{
    if (ee != p_textureCMPreviewer)
    {
        auto Refresh = [this](
            UIInputFile *inputFile, TextureCubeMap *tcm, GL::CubeMapDir cmdir) {
            if (inputFile->GetPath().IsFile())
            {
                Image img;
                RH<Texture2D> tex = tcm->GetSideTexture(cmdir);
                ImageIO::Import(inputFile->GetPath(), &img, tex.Get());
                tcm->SetSideTexture(cmdir, tex.Get());
            }
            else
            {
                tcm->SetSideTexture(cmdir, nullptr);
            }
        };

        TextureCubeMap *tcm = GetTextureCubeMap();
        Refresh(p_topTextureInput, tcm, GL::CubeMapDir::TOP);
        Refresh(p_botTextureInput, tcm, GL::CubeMapDir::BOT);
        Refresh(p_leftTextureInput, tcm, GL::CubeMapDir::LEFT);
        Refresh(p_rightTextureInput, tcm, GL::CubeMapDir::RIGHT);
        Refresh(p_frontTextureInput, tcm, GL::CubeMapDir::FRONT);
        Refresh(p_backTextureInput, tcm, GL::CubeMapDir::BACK);

        CheckValidity();
    }
}
