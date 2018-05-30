#ifndef UIINPUTMATERIAL_H
#define UIINPUTMATERIAL_H

#include "Bang/Bang.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/UIInputFile.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UIInputMaterial : public UIInputFile
{
public:
	UIInputMaterial();
	virtual ~UIInputMaterial();

    void SetPath(const Path &path) override;

private:
    UIImageRenderer *p_previewImg = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // UIINPUTMATERIAL_H

