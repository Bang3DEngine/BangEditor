#ifndef CIWMESHRENDERER_H
#define CIWMESHRENDERER_H

#include "BangEditor/CIWRenderer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class MeshRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputFile;

class CIWMeshRenderer : public CIWRenderer
{
    GAMEOBJECT_EDITOR(CIWMeshRenderer);

public:
    void Update() override;

private:
    MeshRenderer *p_relatedMeshRenderer = nullptr;

    UIInputFile *p_meshInputFile = nullptr;

    MeshRenderer *GetMeshRenderer() const;

    CIWMeshRenderer();
    virtual ~CIWMeshRenderer();

    // IValueChangedListener
    void OnValueChanged(Object *object) override;

    // ComponentWidget
    void SetComponent(Component *comp) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWMESHRENDERER_H

