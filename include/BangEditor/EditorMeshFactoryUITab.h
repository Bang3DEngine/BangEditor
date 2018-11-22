#ifndef EDITORMESHFACTORYUITAB_H
#define EDITORMESHFACTORYUITAB_H

#include "Bang/Bang.h"
#include "Bang/GameObject.h"
#include "BangEditor/BangEditor.h"

using namespace Bang;
namespace Bang
{
class UIButton;
}

namespace BangEditor
{
class EditorMeshFactoryUITabScene;
class EditorMeshFactoryUITab : public GameObject
{
    GAMEOBJECT_EDITOR(EditorMeshFactoryUITab);

public:
    EditorMeshFactoryUITab();
    virtual ~EditorMeshFactoryUITab();

private:
    GameObject *p_sidebar = nullptr;
    UIButton *p_centerSceneButton = nullptr;
    EditorMeshFactoryUITabScene *p_meshFactoryUITabScene = nullptr;
};
}

#endif  // EDITORMESHFACTORYUITAB_H
