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

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

private:
    UIInputFile *p_meshInputFile = nullptr;

    MeshRenderer *GetMeshRenderer() const;

    CIWMeshRenderer() = default;
    virtual ~CIWMeshRenderer() = default;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWMESHRENDERER_H

