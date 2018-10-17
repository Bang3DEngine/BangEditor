#ifndef CIWSKINNEDMESHRENDERER_H
#define CIWSKINNEDMESHRENDERER_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWMeshRenderer.h"

using namespace Bang;
namespace BangEditor
{
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
}

#endif  // CIWSKINNEDMESHRENDERER_H
