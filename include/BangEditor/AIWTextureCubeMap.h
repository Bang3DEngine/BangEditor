#ifndef AIWTEXTURECUBEMAP_H
#define AIWTEXTURECUBEMAP_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/AssetHandle.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/IEvents.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/TextureCubeMap.h"
#include "BangEditor/AIWAsset.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class IEventsValueChanged;
class Texture2D;
class UILabel;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UIInputFile;
class UITextureCubeMapPreviewer;

class AIWTextureCubeMap : public AIWAsset<TextureCubeMap>
{
    GAMEOBJECT_EDITOR(AIWTextureCubeMap);

public:
    AIWTextureCubeMap();

    // InspectorWidget
    void Init() override;

private:
    UIInputFile *p_topTextureInput = nullptr;
    UIInputFile *p_botTextureInput = nullptr;
    UIInputFile *p_rightTextureInput = nullptr;
    UIInputFile *p_leftTextureInput = nullptr;
    UIInputFile *p_frontTextureInput = nullptr;
    UIInputFile *p_backTextureInput = nullptr;
    UITextureCubeMapPreviewer *p_textureCMPreviewer = nullptr;
    UILabel *p_warningLabel = nullptr;

    virtual ~AIWTextureCubeMap() override;

    TextureCubeMap *GetTextureCubeMap() const;

    void CheckValidity() const;

    // AIWAsset
    void UpdateInputsFromAsset() override;
    Texture2D *GetIconTexture() const override;

    // AIWAsset
    void OnValueChangedAIWAsset(
        EventEmitter<IEventsValueChanged> *object) override;
};
}

#endif  // AIWTEXTURECUBEMAP_H
