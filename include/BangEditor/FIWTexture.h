#ifndef FIWTEXTURE_H
#define FIWTEXTURE_H

#include "Bang/Path.h"
#include "Bang/Texture2D.h"
#include "Bang/ResourceHandle.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/FIWResource.h"

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

class FIWTexture : public FIWResource<Texture2D>
{
    GAMEOBJECT_EDITOR(FIWTexture);

public:
    // InspectorWidget
    void Init() override;

private:
    UIComboBox *p_filterModeComboBox = nullptr;
    UIComboBox *p_wrapModeComboBox = nullptr;
    UISlider *p_alphaCutoffInput = nullptr;
    UICheckBox *p_SRGBCheckBoxInput = nullptr;

    UIImageRenderer *p_textureImageRend = nullptr;
    UIAspectRatioFitter *p_imageAspectRatioFitter = nullptr;

	FIWTexture();
	virtual ~FIWTexture();

    Texture2D *GetTexture() const;

    // FIWResource
    void UpdateInputsFromResource() override;

    // FIWResource
    void OnValueChangedFIWResource(EventEmitter<IEventsValueChanged> *object)
                                                                    override;

    friend class FileInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // FIWTEXTURE_H

