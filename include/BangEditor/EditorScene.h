#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "BangEditor/BangEditor.h"

#include "Bang/Scene.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD class Texture2D;
FORWARD class UIGameObject;
FORWARD class UITextRenderer;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class Console;
FORWARD class MenuBar;
FORWARD class Explorer;
FORWARD class Inspector;
FORWARD class Hierarchy;
FORWARD class UISceneContainer;

class EditorScene : public Scene
{
public:
    EditorScene();
    virtual ~EditorScene();

    void Update() override;
    void Render(RenderPass rp, bool renderChildren) override;
    void RenderOpenScene();

    void SetOpenScene(Scene *openScene);
    Scene *GetOpenScene() const;

private:
    Console *m_console     = nullptr;
    Explorer *m_explorer   = nullptr;
    Inspector *m_inspector = nullptr;
    Hierarchy *m_hierarchy = nullptr;

    Scene *p_openScene = nullptr;
    UISceneContainer *m_sceneContainer = nullptr;

    MenuBar *m_menuBar = nullptr;

    UITextRenderer *m_noSceneText = nullptr;
    UIImageRenderer *m_noSceneImg = nullptr;
    UIGameObject *m_sceneContainerGo = nullptr;

    friend class EditorApplication;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORSCENE_H
