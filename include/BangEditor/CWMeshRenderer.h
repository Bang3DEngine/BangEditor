#ifndef CWMESHRENDERER_H
#define CWMESHRENDERER_H

#include "BangEditor/CWRenderer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class MeshRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputFile;

class CWMeshRenderer : public CWRenderer
{
    GAMEOBJECT_EDITOR(CWMeshRenderer);

public:
    void Update() override;

private:
    MeshRenderer *p_relatedMeshRenderer = nullptr;

    UIInputFile *p_meshInputFile = nullptr;

    MeshRenderer *GetMeshRenderer() const;

    CWMeshRenderer();
    virtual ~CWMeshRenderer();

    // IValueChangedListener
    void OnValueChanged(Object *object) override;

    // ComponentWidget
    void SetComponent(Component *comp) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // CWMESHRENDERER_H

