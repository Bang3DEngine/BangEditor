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

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class IEventsValueChanged;
FORWARD   class Texture2D;
FORWARD   class UILabel;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputTexture;
FORWARD class UITextureCubeMapPreviewer;

class RIWTextureCubeMap : public RIWResource<TextureCubeMap>
{
    GAMEOBJECT_EDITOR(RIWTextureCubeMap);

public:
    // InspectorWidget
    void Init() override;

private:
    UIInputTexture *p_topTextureInput   = nullptr;
    UIInputTexture *p_botTextureInput   = nullptr;
    UIInputTexture *p_rightTextureInput = nullptr;
    UIInputTexture *p_leftTextureInput  = nullptr;
    UIInputTexture *p_frontTextureInput = nullptr;
    UIInputTexture *p_backTextureInput  = nullptr;
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
    void OnValueChangedRIWResource(EventEmitter<IEventsValueChanged> *object)
                                                                    override;

};

NAMESPACE_BANG_EDITOR_END

#endif // RIWTEXTURECUBEMAP_H

