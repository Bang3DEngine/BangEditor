#ifndef UIINPUTTEXTURE_H
#define UIINPUTTEXTURE_H

#include "Bang/Bang.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/UIInputFileWithPreview.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UIInputTexture : public UIInputFileWithPreview
{
public:
	UIInputTexture();
	virtual ~UIInputTexture();

    virtual RH<Texture2D> GetPreviewTextureFromPath(const Path &path) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UIINPUTTEXTURE_H

