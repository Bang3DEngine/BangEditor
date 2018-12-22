#include "BangEditor/AIWTextureCubeMap.h"

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
#include "Bang/AssetHandle.h"
#include "Bang/Texture2D.h"
#include "Bang/UILabel.h"
#include "Bang/UITextRenderer.h"
#include "BangEditor/AIWAsset.tcc"
#include "BangEditor/AssetInspectorWidget.h"
#include "BangEditor/UIInputFile.h"
#include "BangEditor/UITextureCubeMapPreviewer.h"

namespace Bang
{
class IEventsValueChanged;
}

using namespace Bang;
using namespace BangEditor;

AIWTextureCubeMap::AIWTextureCubeMap()
{
}

AIWTextureCubeMap::~AIWTextureCubeMap()
{
}

void AIWTextureCubeMap::Init()
{
    AssetInspectorWidget::Init();

    SetTitle("Texture Cube Map");
    SetName("AIWTextureCubeMap");

    p_topTextureInput = new UIInputFile();
    p_topTextureInput->SetExtensions(Extensions::GetImageExtensions());
    p_topTextureInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    AddWidget("Top texture", p_topTextureInput);

    p_botTextureInput = new UIInputFile();
    p_botTextureInput->SetExtensions(Extensions::GetImageExtensions());
    p_botTextureInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    AddWidget("Bot texture", p_botTextureInput);

    p_leftTextureInput = new UIInputFile();
    p_leftTextureInput->SetExtensions(Extensions::GetImageExtensions());
    p_leftTextureInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    AddWidget("Left texture", p_leftTextureInput);

    p_rightTextureInput = new UIInputFile();
    p_rightTextureInput->SetExtensions(Extensions::GetImageExtensions());
    p_rightTextureInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    AddWidget("Right texture", p_rightTextureInput);

    p_frontTextureInput = new UIInputFile();
    p_frontTextureInput->SetExtensions(Extensions::GetImageExtensions());
    p_frontTextureInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    AddWidget("Front texture", p_frontTextureInput);

    p_backTextureInput = new UIInputFile();
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

TextureCubeMap *AIWTextureCubeMap::GetTextureCubeMap() const
{
    return SCAST<TextureCubeMap *>(GetAsset().Get());
}

void AIWTextureCubeMap::CheckValidity() const
{
    TextureCubeMap *tcm = GetTextureCubeMap();
    AH<Texture2D> topTex = tcm->GetSideTexture(GL::CubeMapDir::TOP);
    AH<Texture2D> botTex = tcm->GetSideTexture(GL::CubeMapDir::BOT);
    AH<Texture2D> leftTex = tcm->GetSideTexture(GL::CubeMapDir::LEFT);
    AH<Texture2D> rightTex = tcm->GetSideTexture(GL::CubeMapDir::RIGHT);
    AH<Texture2D> frontTex = tcm->GetSideTexture(GL::CubeMapDir::FRONT);
    AH<Texture2D> backTex = tcm->GetSideTexture(GL::CubeMapDir::BACK);

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

void AIWTextureCubeMap::UpdateInputsFromAsset()
{
    TextureCubeMap *tcm = GetTextureCubeMap();
    const AH<Texture2D> topTex = tcm->GetSideTexture(GL::CubeMapDir::TOP);
    const AH<Texture2D> botTex = tcm->GetSideTexture(GL::CubeMapDir::BOT);
    const AH<Texture2D> leftTex = tcm->GetSideTexture(GL::CubeMapDir::LEFT);
    const AH<Texture2D> rightTex = tcm->GetSideTexture(GL::CubeMapDir::RIGHT);
    const AH<Texture2D> frontTex = tcm->GetSideTexture(GL::CubeMapDir::FRONT);
    const AH<Texture2D> backTex = tcm->GetSideTexture(GL::CubeMapDir::BACK);
    p_topTextureInput->SetPath(topTex ? topTex.Get()->GetAssetFilepath()
                                      : Path::Empty());
    p_botTextureInput->SetPath(botTex ? botTex.Get()->GetAssetFilepath()
                                      : Path::Empty());
    p_leftTextureInput->SetPath(leftTex ? leftTex.Get()->GetAssetFilepath()
                                        : Path::Empty());
    p_rightTextureInput->SetPath(
        rightTex ? rightTex.Get()->GetAssetFilepath() : Path::Empty());
    p_frontTextureInput->SetPath(
        frontTex ? frontTex.Get()->GetAssetFilepath() : Path::Empty());
    p_backTextureInput->SetPath(backTex ? backTex.Get()->GetAssetFilepath()
                                        : Path::Empty());

    p_textureCMPreviewer->SetTextureCubeMap(tcm);

    CheckValidity();
}

Texture2D *AIWTextureCubeMap::GetIconTexture() const
{
    return GetTextureCubeMap()
               ? GetTextureCubeMap()
                     ->GetSideTexture(GL::CubeMapDir::FRONT)
                     .Get()
               : nullptr;
}

void AIWTextureCubeMap::OnValueChangedAIWAsset(
    EventEmitter<IEventsValueChanged> *ee)
{
    if (ee != p_textureCMPreviewer)
    {
        auto Refresh = [this](
            UIInputFile *inputFile, TextureCubeMap *tcm, GL::CubeMapDir cmdir) {
            if (inputFile->GetPath().IsFile())
            {
                Image img;
                AH<Texture2D> tex = tcm->GetSideTexture(cmdir);
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
