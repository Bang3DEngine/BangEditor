#ifndef FIWTEXTURECUBEMAP_H
#define FIWTEXTURECUBEMAP_H

#include "Bang/Path.h"
#include "Bang/ResourceHandle.h"
#include "Bang/TextureCubeMap.h"
#include "Bang/IValueChangedListener.h"

#include "BangEditor/FileInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Texture2D;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputTexture;

class FIWTextureCubeMap : public FileInspectorWidget,
                          public EventListener<IValueChangedListener>
{
    GAMEOBJECT_EDITOR(FIWTextureCubeMap);

public:
    // InspectorWidget
    void Init() override;

private:
    RH<TextureCubeMap> p_textureCubeMap;
    UIInputTexture *p_topTextureInput   = nullptr;
    UIInputTexture *p_botTextureInput   = nullptr;
    UIInputTexture *p_rightTextureInput = nullptr;
    UIInputTexture *p_leftTextureInput  = nullptr;
    UIInputTexture *p_frontTextureInput = nullptr;
    UIInputTexture *p_backTextureInput  = nullptr;
    UILabel *p_warningLabel = nullptr;

    FIWTextureCubeMap();
    virtual ~FIWTextureCubeMap();

    TextureCubeMap *GetTextureCubeMap() const;

    void CheckValidity() const;

    // FileInspectorWidget
    void UpdateFromFileWhenChanged() override;

    // IValueChangedListener
    void OnValueChanged(Object *object) override;

};

NAMESPACE_BANG_EDITOR_END

#endif // FIWTEXTURECUBEMAP_H

