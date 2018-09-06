#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include <stack>

#include "Bang/Scene.h"
#include "Bang/AARect.h"
#include "Bang/SceneManager.h"
#include "Bang/IEventsDestroy.h"

#include "BangEditor/Editor.h"
#include "BangEditor/ScenePlayer.h"
#include "BangEditor/EditorWindow.h"
#include "BangEditor/IEventsScenePlayer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD class Texture2D;
FORWARD class GameObject;
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
FORWARD class ScenePlayer;
FORWARD class ProjectManager;
FORWARD class UITabContainer;
FORWARD class EditorClipboard;
FORWARD class UndoRedoManager;
FORWARD class SceneOpenerSaver;
FORWARD class UISceneContainer;
FORWARD class EditorApplication;
FORWARD class EditorFileTracker;
FORWARD class EditSceneGameObjects;
FORWARD class UISceneEditContainer;
FORWARD class UIScenePlayContainer;

class EditorScene : public Scene,
                    public EventListener<IEventsWindow>,
                    public EventListener<IEventsScenePlayer>,
                    public EventListener<IEventsSceneManager>
{
    GAMEOBJECT_EDITOR(EditorScene);

public:
    void Update() override;
    void OnResize(int newWidth, int newHeight) override;

    void RenderOpenSceneIfNeeded();
    void SetViewportForOpenScene();

    Scene *GetOpenScene() const;

    AARect GetOpenSceneWindowRectNDC() const;

    MenuBar *GetMenuBar() const;
    Console *GetConsole() const;
    Explorer *GetExplorer() const;
    Inspector *GetInspector() const;
    Hierarchy *GetHierarchy() const;
    ScenePlayer *GetScenePlayer() const;
    ProjectManager *GetProjectManager() const;
    EditorClipboard *GetEditorClipboard() const;
    UndoRedoManager *GetUndoRedoManager() const;
    UITabContainer *GetSceneTabContainer() const;
    SceneOpenerSaver *GetSceneOpenerSaver() const;
    EditorFileTracker *GetEditorFileTracker() const;
    UISceneEditContainer *GetSceneEditContainer() const;
    UIScenePlayContainer *GetScenePlayContainer() const;
    EditSceneGameObjects *GetEditSceneGameObjects() const;

protected:
    EditorScene();
    virtual ~EditorScene();

private:
    EditorFileTracker *m_editorFileTracker = nullptr;

    ScenePlayer *m_scenePlayer = nullptr;
    ProjectManager *m_projectManager = nullptr;
    EditorClipboard *m_editorClipboard = nullptr;
    SceneOpenerSaver *m_sceneOpenerSaver = nullptr;
    EditSceneGameObjects *m_editSceneGameObjects = nullptr;

    Console *p_console     = nullptr;
    Explorer *p_explorer   = nullptr;
    Inspector *p_inspector = nullptr;
    Hierarchy *p_hierarchy = nullptr;
    UndoRedoManager *m_undoRedoManager = nullptr;

    Scene *p_openScene = nullptr;
    UISceneEditContainer *p_sceneEditContainer = nullptr;
    UIScenePlayContainer *p_scenePlayContainer = nullptr;

    UITabContainer *p_topLeftTabContainer = nullptr;
    UITabContainer *p_topCenterTabContainer = nullptr;
    UITabContainer *p_topRightTabContainer = nullptr;
    UITabContainer *p_botLeftTabContainer = nullptr;
    UITabContainer *p_botRightTabContainer = nullptr;

    MenuBar *m_menuBar = nullptr;
    GameObject *m_mainEditorVLGo = nullptr;

    std::stack<AARecti> m_viewportsStack;

    void Init();

    // GameObject
    void BeforeRender() override;

    void BindOpenScene();
    void UnBindOpenScene();
    void PushGLViewport();
    void PopGLViewport();

    void SetOpenScene(Scene *openScene);

    // IEventsWindow
    void OnFocusGained(Window *w) override;
    void OnFocusLost(Window *w) override;

    // IEventsSceneManager
    void OnSceneLoaded(Scene *scene, const Path &sceneFilepath) override;

    // IEventsScenePlayer
    void OnPlayStateChanged(PlayState previousPlayState,
                            PlayState newPlayState) override;

    // IEventsDestroy
    void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;

    friend class EditorApplication;
    friend class EditorSceneManager;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORSCENE_H
