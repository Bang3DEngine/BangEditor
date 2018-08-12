#ifndef UITEXTURECUBEMAPPREVIEWER_H
#define UITEXTURECUBEMAPPREVIEWER_H

#include "Bang/Bang.h"
#include "Bang/Texture2D.h"
#include "Bang/GameObject.h"
#include "Bang/UIFocusable.h"
#include "Bang/EventEmitter.h"
#include "Bang/ResourceHandle.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/ResourcePreviewFactory.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIImageRenderer;
FORWARD class UIAspectRatioFitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UITextureCubeMapPreviewer : public GameObject,
                                  public EventEmitter<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR(UITextureCubeMapPreviewer);

public:
	UITextureCubeMapPreviewer();
	virtual ~UITextureCubeMapPreviewer();

    // GameObject
    void Update() override;

    void SetTextureCubeMap(TextureCubeMap *texCubeMap);
    TextureCubeMap *GetTextureCubeMap() const;

private:
    UIFocusable *p_focusable = nullptr;
    RH<TextureCubeMap> p_textureCubeMap;
    Vector2 m_rotationRads = Vector2::Zero;
    RH<Material> p_previewImageRendererMaterial;
    UIImageRenderer *p_previewImageRenderer = nullptr;
    UIAspectRatioFitter *p_previewImgAspectRatioFitter = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // UITEXTURECUBEMAPPREVIEWER_H

