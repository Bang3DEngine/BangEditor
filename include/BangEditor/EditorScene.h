#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "Bang/Rect.h"
#include "Bang/Scene.h"

#include "BangEditor/Editor.h"
#include "BangEditor/IEditorOpenSceneListener.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD class Texture2D;
FORWARD class GameObject;
FORWARD class UITextRenderer;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class Editor;
FORWARD class Console;
FORWARD class MenuBar;
FORWARD class Explorer;
FORWARD class Inspector;
FORWARD class Hierarchy;
FORWARD class ScenePlayer;
FORWARD class ProjectManager;
FORWARD class UITabContainer;
FORWARD class UISceneContainer;
FORWARD class EditorApplication;
FORWARD class EditSceneGameObjects;
FORWARD class UISceneEditContainer;
FORWARD class UIScenePlayContainer;
FORWARD class EditorBehaviourManager;

class EditorScene : public Scene,
                    public IEditorListener,
                    public EventEmitter<IEditorOpenSceneListener>
{
    GAMEOBJECT_EDITOR(EditorScene);

public:
    void Update() override;
    void OnResize(int newWidth, int newHeight) override;

    void RenderOpenScene();
    void SetViewportForOpenScene();

    void SetOpenScene(Scene *openScene, bool destroyPreviousScene = true);
    Scene *GetOpenScene() const;

    Rect GetOpenSceneScreenRectNDC() const;

    void RenderAndBlitToScreen();

    MenuBar *GetMenuBar() const;
    Editor *GetEditor() const;
    Console *GetConsole() const;
    Explorer *GetExplorer() const;
    Inspector *GetInspector() const;
    Hierarchy *GetHierarchy() const;
    ScenePlayer *GetScenePlayer() const;
    ProjectManager *GetProjectManager() const;
    UITabContainer *GetSceneTabContainer() const;
    UISceneEditContainer *GetSceneEditContainer() const;
    UIScenePlayContainer *GetScenePlayContainer() const;
    EditorBehaviourManager *GetBehaviourManager() const;
    EditSceneGameObjects *GetEditSceneGameObjects() const;

protected:
    EditorScene();
    virtual ~EditorScene();

private:
    Editor *m_editor = nullptr;
    ScenePlayer *m_scenePlayer = nullptr;
    ProjectManager *m_projectManager = nullptr;
    EditorBehaviourManager *m_behaviourManager = nullptr;
    EditSceneGameObjects *m_editSceneGameObjects = nullptr;

    Console *p_console     = nullptr;
    Explorer *p_explorer   = nullptr;
    Inspector *p_inspector = nullptr;
    Hierarchy *p_hierarchy = nullptr;

    Scene *p_openScene = nullptr;
    UITabContainer *p_sceneTabContainer = nullptr;
    UISceneEditContainer *p_sceneEditContainer = nullptr;
    UIScenePlayContainer *p_scenePlayContainer = nullptr;

    MenuBar *m_menuBar = nullptr;
    GameObject *m_mainEditorVL = nullptr;

    Recti m_prevGLViewport = Recti::Zero;

    void Init();

    void BindOpenScene();
    void UnBindOpenScene();
    void PushGLViewport();
    void PopGLViewport();

    // IEditorListener
    void OnPlayStateChanged(EditorPlayState previousPlayState,
                            EditorPlayState newPlayState) override;

    friend class BangEditor::EditorApplication;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORSCENE_H
