#ifndef EDITORFLOOR_H
#define EDITORFLOOR_H

#include "Bang/Bang.h"
#include "Bang/GameObject.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class LineRenderer;
FORWARD NAMESPACE_BANG_END


USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class EditorFloor : public GameObject
{
    GAMEOBJECT(EditorFloor);

public:
	EditorFloor();
	virtual ~EditorFloor();

    void Render(RenderPass renderPass, bool renderChildren) override;

private:
    LineRenderer *m_lineRenderer = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORFLOOR_H

