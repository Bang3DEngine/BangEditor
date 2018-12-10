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
class UIInputFile;
class UITextureCubeMapPreviewer;

class RIWTextureCubeMap : public RIWResource<TextureCubeMap>
{
    GAMEOBJECT_EDITOR(RIWTextureCubeMap);

public:
    RIWTextureCubeMap();

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
