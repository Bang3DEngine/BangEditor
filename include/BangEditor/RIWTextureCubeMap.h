#ifndef RIWTEXTURECUBEMAP_H
#define RIWTEXTURECUBEMAP_H

#include "Bang/Path.h"
#include "Bang/ResourceHandle.h"
#include "Bang/TextureCubeMap.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/RIWResource.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Texture2D;
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
    virtual ~RIWTextureCubeMap();

    TextureCubeMap *GetTextureCubeMap() const;

    void CheckValidity() const;

    // RIWResource
    void UpdateInputsFromResource() override;

    // RIWResource
    void OnValueChangedRIWResource(EventEmitter<IEventsValueChanged> *object)
                                                                    override;

};

NAMESPACE_BANG_EDITOR_END

#endif // RIWTEXTURECUBEMAP_H

