#ifndef ANIMATOREDITOR_H
#define ANIMATOREDITOR_H

#include "Bang/Bang.h"
#include "Bang/GameObject.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class AnimatorEditor : public GameObject
{
    GAMEOBJECT_EDITOR(AnimatorEditor);

public:
	AnimatorEditor();
	virtual ~AnimatorEditor();

    // GameObject
    void Update() override;
};

NAMESPACE_BANG_EDITOR_END

#endif // ANIMATOREDITOR_H

