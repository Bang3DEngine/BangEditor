#ifndef UISCENEDEBUGSTATS_H
#define UISCENEDEBUGSTATS_H

#include "Bang/Array.h"
#include "Bang/BangDefines.h"
#include "Bang/FPSChrono.h"
#include "Bang/GameObject.h"
#include "Bang/Scene.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class Object;
class UITextRenderer;
}

using namespace Bang;
namespace BangEditor
{
class UISceneDebugStats : public GameObject
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(UISceneDebugStats);

public:
    UISceneDebugStats();
    virtual ~UISceneDebugStats() override;

    // GameObject
    void Update() override;

    // IEventsObject
    void OnEnabled(Object *object) override;

private:
    FPSChrono m_editorRenderFPSChrono;
    UITextRenderer *p_debugStatsText = nullptr;
};
}

#endif  // UISCENEDEBUGSTATS_H
