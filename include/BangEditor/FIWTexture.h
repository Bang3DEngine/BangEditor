#ifndef FIWTEXTURE_H
#define FIWTEXTURE_H

#include "Bang/Path.h"
#include "Bang/ResourceHandle.h"
#include "Bang/IValueChangedListener.h"

#include "BangEditor/FileInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UISlider;
FORWARD class Texture2D;
FORWARD class UIComboBox;
FORWARD class UIImageRenderer;
FORWARD class UIAspectRatioFitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class FIWTexture : public FileInspectorWidget,
                   public IValueChangedListener
{
    GAMEOBJECT_EDITOR(FIWTexture);

public:
    // InspectorWidget
    void Init() override;

private:
    RH<Texture2D> p_texture;

    UIComboBox *p_filterModeComboBox = nullptr;
    UIComboBox *p_wrapModeComboBox = nullptr;
    UISlider *p_alphaCutoffInput = nullptr;

    UIImageRenderer *p_textureImageRend = nullptr;
    UIAspectRatioFitter *p_imageAspectRatioFitter = nullptr;

	FIWTexture();
	virtual ~FIWTexture();

    Texture2D *GetTexture() const;

    // FileInspectorWidget
    void UpdateFromFileWhenChanged() override;

    // IValueChangedListener
    void OnValueChanged(Object *object) override;

    friend class FileInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // FIWTEXTURE_H

