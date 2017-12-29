#ifndef FIWTEXTURE_H
#define FIWTEXTURE_H

#include "Bang/Path.h"
#include "Bang/ResourceHandle.h"
#include "Bang/IValueChangedListener.h"

#include "BangEditor/FileInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIComboBox;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class FIWTexture : public FileInspectorWidget,
                   public IValueChangedListener
{
    GAMEOBJECT_EDITOR(FIWTexture);

private:
    RH<Texture2D> p_texture;

    UIComboBox *p_filterModeComboBox = nullptr;

	FIWTexture();
	virtual ~FIWTexture();

    Texture2D *GetTexture() const;

    void UpdateFromTextureFile();

    // FileInspectorWidget
    void OnPathChanged(const Path &path) override;

    // IValueChangedListener
    void OnValueChanged(Object *object) override;

    friend class FileInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // FIWTEXTURE_H

