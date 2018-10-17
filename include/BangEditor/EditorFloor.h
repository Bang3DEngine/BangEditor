#ifndef EDITORFLOOR_H
#define EDITORFLOOR_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "Bang/RenderPass.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class LineRenderer;
}

using namespace Bang;
namespace BangEditor
{
class EditorFloor : public GameObject
{
    GAMEOBJECT(EditorFloor);

public:
    EditorFloor();
    virtual ~EditorFloor() override;

    void Render(RenderPass renderPass, bool renderChildren) override;

private:
    LineRenderer *m_lineRenderer = nullptr;
};
}

#endif  // EDITORFLOOR_H
