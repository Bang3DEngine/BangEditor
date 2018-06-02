#ifndef FIWTEXTURE_H
#define FIWTEXTURE_H

#include "Bang/Path.h"
#include "Bang/ResourceHandle.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/IEventsTextureChanged.h"

#include "BangEditor/FileInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UISlider;
FORWARD class Texture2D;
FORWARD class UIComboBox;
FORWARD class UICheckBox;
FORWARD class UIImageRenderer;
FORWARD class UIAspectRatioFitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class FIWTexture : public FileInspectorWidget,
                   public EventListener<IEventsTextureChanged>,
                   public EventListener<IEventsValueChanged>
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
    UICheckBox *p_SRGBCheckBoxInput = nullptr;

    UIImageRenderer *p_textureImageRend = nullptr;
    UIAspectRatioFitter *p_imageAspectRatioFitter = nullptr;

	FIWTexture();
	virtual ~FIWTexture();

    Texture2D *GetTexture() const;
    void UpdateInputsFromTexture();

    // FileInspectorWidget
    void UpdateFromFileWhenChanged() override;

    // IEventsTextureChanged
    void OnTextureChanged(const Texture *changedTexture) override;

    // IEventsValueChanged
    void OnValueChanged(EventEmitter<IEventsValueChanged> *object) override;

    friend class FileInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // FIWTEXTURE_H

