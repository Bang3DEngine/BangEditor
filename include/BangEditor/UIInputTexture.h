#ifndef UIINPUTTEXTURE_H
#define UIINPUTTEXTURE_H

#include "Bang/Bang.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/UIInputFile.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UIInputTexture : public UIInputFile
{
public:
	UIInputTexture();
	virtual ~UIInputTexture();

    void SetPath(const Path &path) override;

private:
    UIImageRenderer *p_previewImg = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // UIINPUTTEXTURE_H

