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

FORWARD class UIInputFile;

class FIWTextureCubeMap : public FileInspectorWidget,
                          public IValueChangedListener
{
    GAMEOBJECT_EDITOR(FIWTextureCubeMap);

public:
    // InspectorWidget
    void Init() override;

private:
    RH<TextureCubeMap> p_textureCubeMap;
    UIInputFile *p_topTextureInput   = nullptr;
    UIInputFile *p_botTextureInput   = nullptr;
    UIInputFile *p_rightTextureInput = nullptr;
    UIInputFile *p_leftTextureInput  = nullptr;
    UIInputFile *p_frontTextureInput = nullptr;
    UIInputFile *p_backTextureInput  = nullptr;
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

