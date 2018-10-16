#ifndef UISCENEDEBUGSTATS_H
#define UISCENEDEBUGSTATS_H

#include "Bang/Array.h"
#include "Bang/BangDefines.h"
#include "Bang/FPSChrono.h"
#include "Bang/GameObject.h"
#include "Bang/Scene.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

namespace Bang {
class Object;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UITextRenderer;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UISceneDebugStats : public GameObject
{
    GAMEOBJECT_EDITOR(UISceneDebugStats);

public:
	UISceneDebugStats();
	virtual ~UISceneDebugStats();

    // GameObject
    void Update() override;

    // IEventsObject
    void OnEnabled(Object *object) override;

private:
    FPSChrono m_editorRenderFPSChrono;
    UITextRenderer *p_debugStatsText = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // UISCENEDEBUGSTATS_H

