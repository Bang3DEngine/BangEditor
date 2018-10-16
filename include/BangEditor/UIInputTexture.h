#ifndef UIINPUTTEXTURE_H
#define UIINPUTTEXTURE_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/ResourceHandle.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/UIInputFileWithPreview.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Path;
FORWARD class Texture2D;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UIInputTexture : public UIInputFileWithPreview
{
public:
	UIInputTexture();
	virtual ~UIInputTexture() override;

    virtual RH<Texture2D> GetPreviewTextureFromPath(const Path &path) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UIINPUTTEXTURE_H

