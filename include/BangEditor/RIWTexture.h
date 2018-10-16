#ifndef RIWTEXTURE_H
#define RIWTEXTURE_H

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
#include "Bang/Texture2D.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/RIWResource.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class IEventsValueChanged;
FORWARD   class Texture2D;
FORWARD   class UIAspectRatioFitter;
FORWARD   class UICheckBox;
FORWARD   class UIComboBox;
FORWARD   class UIImageRenderer;
FORWARD   class UIInputText;
FORWARD   class UISlider;
FORWARD_T class EventEmitter;
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

    UIInputText *p_textureWidth  = nullptr;
    UIInputText *p_textureHeight = nullptr;
    UIImageRenderer *p_textureImageRend = nullptr;
    UIAspectRatioFitter *p_imageAspectRatioFitter = nullptr;

	RIWTexture();
	virtual ~RIWTexture() override;

    Texture2D *GetTexture() const;

    // RIWResource
    void UpdateInputsFromResource() override;
    Texture2D *GetIconTexture() const override;

    // RIWResource
    void OnValueChangedRIWResource(EventEmitter<IEventsValueChanged> *object)
                                                                    override;

    friend class ResourceInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // RIWTEXTURE_H

