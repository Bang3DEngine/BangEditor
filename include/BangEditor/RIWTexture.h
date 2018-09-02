#ifndef RIWTEXTURE_H
#define RIWTEXTURE_H

#include "Bang/Path.h"
#include "Bang/Texture2D.h"
#include "Bang/ResourceHandle.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/RIWResource.h"

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

class RIWTexture : public RIWResource<Texture2D>
{
    GAMEOBJECT_EDITOR(RIWTexture);

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

	RIWTexture();
	virtual ~RIWTexture();

    Texture2D *GetTexture() const;

    // RIWResource
    void UpdateInputsFromResource() override;

    // RIWResource
    void OnValueChangedRIWResource(EventEmitter<IEventsValueChanged> *object)
                                                                    override;

    friend class ResourceInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // RIWTEXTURE_H

