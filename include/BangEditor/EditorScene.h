#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "BangEditor/BangEditor.h"

#include "Bang/Scene.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD class UIGameObject;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class MenuBar;

class EditorScene : public Scene
{
public:
    EditorScene();
    virtual ~EditorScene();

    void SetOpenScene(Scene *openScene);

    Scene *GetOpenScene() const;

private:
    Scene *p_openScene = nullptr;

    MenuBar *m_menuBar = nullptr;

    UIGameObject *m_openSceneContainer = nullptr;
    UIImageRenderer *m_openSceneImage  = nullptr;

    friend class EditorApplication;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORSCENE_H
