#ifndef SCENEDEBUGSTATS_H
#define SCENEDEBUGSTATS_H

#include "Bang/Array.h"
#include "Bang/Scene.h"
#include "Bang/FPSChrono.h"
#include "Bang/GameObject.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UITextRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class SceneDebugStats : public GameObject
{
    GAMEOBJECT_EDITOR(SceneDebugStats);

public:
	SceneDebugStats();
	virtual ~SceneDebugStats();

    // GameObject
    void Update() override;

private:
    FPSChrono m_editorRenderFPSChrono;
    UITextRenderer *p_debugStatsText = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // SCENEDEBUGSTATS_H

