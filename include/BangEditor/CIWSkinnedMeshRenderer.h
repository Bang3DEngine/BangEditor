#ifndef CIWSKINNEDMESHRENDERER_H
#define CIWSKINNEDMESHRENDERER_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWMeshRenderer.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class CIWSkinnedMeshRenderer : public CIWMeshRenderer
{
    GAMEOBJECT_EDITOR(CIWMeshRenderer);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;

protected:
	CIWSkinnedMeshRenderer();
	virtual ~CIWSkinnedMeshRenderer() override;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWSKINNEDMESHRENDERER_H

