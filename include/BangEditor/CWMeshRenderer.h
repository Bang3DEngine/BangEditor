#ifndef CWMESHRENDERER_H
#define CWMESHRENDERER_H

#include "Bang/IValueChangedListener.h"

#include "BangEditor/ComponentWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class MeshRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputFile;

class CWMeshRenderer : public ComponentWidget,
                       public IValueChangedListener
{
public:
    CWMeshRenderer();
	virtual ~CWMeshRenderer();

    void Update() override;

private:
    MeshRenderer *p_relatedMeshRenderer = nullptr;

    UIInputFile *p_meshInputFile = nullptr;

    void OnValueChanged(Object *object) override;
    void SetComponent(Component *comp) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // CWMESHRENDERER_H

