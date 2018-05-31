#ifndef UIINPUTMATERIAL_H
#define UIINPUTMATERIAL_H

#include "Bang/Bang.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/UIInputFileWithPreview.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UIInputMaterial : public UIInputFileWithPreview
{
public:
	UIInputMaterial();
    virtual ~UIInputMaterial();

    virtual RH<Texture2D> GetPreviewTextureFromPath(const Path &path) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UIINPUTMATERIAL_H

