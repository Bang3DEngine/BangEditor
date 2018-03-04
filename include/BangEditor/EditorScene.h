#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "Bang/Scene.h"
#include "Bang/AARect.h"
#include "Bang/SceneManager.h"
#include "Bang/IDestroyListener.h"

#include "BangEditor/Editor.h"
#include "BangEditor/ScenePlayer.h"

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
FORWARD class EditorClipboard;
FORWARD class SceneOpenerSaver;
FORWARD class UISceneContainer;
FORWARD class EditorApplication;
FORWARD class EditorFileTracker;
FORWARD class EditSceneGameObjects;
FORWARD class UISceneEditContainer;
FORWARD class UIScenePlayContainer;

class EditorScene : public Scene,
                    public IScenePlayerListener,
                    public ISceneManagerListener
{
    GAMEOBJECT_EDITOR(EditorScene);

public:
    void Update() override;
    void OnResize(int newWidth, int newHeight) override;

    void RenderOpenScene();
    void SetViewportForOpenScene();

    Scene *GetOpenScene() const;

    AARect GetOpenSceneWindowRectNDC() const;

    MenuBar *GetMenuBar() const;
    Editor *GetEditor() const;
    Console *GetConsole() const;
    Explorer *GetExplorer() const;
    Inspector *GetInspector() const;
    Hierarchy *GetHierarchy() const;
    ScenePlayer *GetScenePlayer() const;
    ProjectManager *GetProjectManager() const;
    EditorClipboard *GetEditorClipboard() const;
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
    Editor *m_editor = nullptr;
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

    Scene *p_openScene = nullptr;
    UISceneEditContainer *p_sceneEditContainer = nullptr;
    UIScenePlayContainer *p_scenePlayContainer = nullptr;

    UITabContainer *p_leftTabContainer = nullptr;
    UITabContainer *p_centerTabContainer = nullptr;
    UITabContainer *p_rightTabContainer = nullptr;
    UITabContainer *p_botTabContainer = nullptr;

    MenuBar *m_menuBar = nullptr;
    GameObject *m_mainEditorVL = nullptr;

    AARecti m_prevGLViewport = AARecti::Zero;

    void Init();

    // GameObject
    void BeforeRender() override;

    void BindOpenScene();
    void UnBindOpenScene();
    void PushGLViewport();
    void PopGLViewport();

    void SetOpenScene(Scene *openScene);

    // ISceneManagerListener
    void OnSceneLoaded(Scene *scene, const Path &sceneFilepath) override;

    // IScenePlayerListener
    void OnPlayStateChanged(PlayState previousPlayState,
                            PlayState newPlayState) override;

    // IDestroyListener
    void OnDestroyed(EventEmitter<IDestroyListener> *object) override;

    friend class EditorApplication;
    friend class EditorSceneManager;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORSCENE_H
