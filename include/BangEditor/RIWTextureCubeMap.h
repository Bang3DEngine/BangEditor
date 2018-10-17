#ifndef RIWTEXTURECUBEMAP_H
#define RIWTEXTURECUBEMAP_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/IEvents.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/Path.h"
#include "Bang/ResourceHandle.h"
#include "Bang/String.h"
#include "Bang/TextureCubeMap.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/RIWResource.h"

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
class UIInputTexture;
class UITextureCubeMapPreviewer;

class RIWTextureCubeMap : public RIWResource<TextureCubeMap>
{
    GAMEOBJECT_EDITOR(RIWTextureCubeMap);

public:
    // InspectorWidget
    void Init() override;

private:
    UIInputTexture *p_topTextureInput = nullptr;
    UIInputTexture *p_botTextureInput = nullptr;
    UIInputTexture *p_rightTextureInput = nullptr;
    UIInputTexture *p_leftTextureInput = nullptr;
    UIInputTexture *p_frontTextureInput = nullptr;
    UIInputTexture *p_backTextureInput = nullptr;
    UITextureCubeMapPreviewer *p_textureCMPreviewer = nullptr;
    UILabel *p_warningLabel = nullptr;

    RIWTextureCubeMap();
    virtual ~RIWTextureCubeMap() override;

    TextureCubeMap *GetTextureCubeMap() const;

    void CheckValidity() const;

    // RIWResource
    void UpdateInputsFromResource() override;
    Texture2D *GetIconTexture() const override;

    // RIWResource
    void OnValueChangedRIWResource(
        EventEmitter<IEventsValueChanged> *object) override;
};
}

#endif  // RIWTEXTURECUBEMAP_H
