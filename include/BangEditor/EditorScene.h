#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include <vector>

#include "Bang/AARect.h"
#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsDestroy.h"
#include "Bang/Scene.h"
#include "Bang/SceneManager.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/Editor.h"
#include "BangEditor/EditorWindow.h"
#include "BangEditor/IEventsScenePlayer.h"
#include "BangEditor/IEventsTabHeader.h"
#include "BangEditor/PlayState.h"
#include "BangEditor/ScenePlayer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class GameObject;
FORWARD   class Scene;
FORWARD   class Texture2D;
FORWARD   class UIImageRenderer;
FORWARD   class UITextRenderer;
FORWARD   class IEventsDestroy;
FORWARD   class IEventsSceneManager;
FORWARD   class IEventsWindow;
FORWARD   class Path;
FORWARD   class Window;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class AnimatorSMEditor;
FORWARD class Console;
FORWARD class EditSceneGameObjects;
FORWARD class EditorApplication;
FORWARD class EditorClipboard;
FORWARD class EditorFileTracker;
FORWARD class Explorer;
FORWARD class Hierarchy;
FORWARD class IEventsScenePlayer;
FORWARD class IEventsTabHeader;
FORWARD class Inspector;
FORWARD class MenuBar;
FORWARD class ProjectManager;
FORWARD class SceneOpenerSaver;
FORWARD class ScenePlayer;
FORWARD class UISceneContainer;
FORWARD class UISceneEditContainer;
FORWARD class UIScenePlayContainer;
FORWARD class UITabContainer;
FORWARD class UITabHeader;
FORWARD class UITabStation;
FORWARD class UndoRedoManager;

class EditorScene : public Scene,
                    public EventListener<IEventsWindow>,
                    public EventListener<IEventsTabHeader>,
                    public EventListener<IEventsScenePlayer>,
                    public EventListener<IEventsSceneManager>
{
    GAMEOBJECT_EDITOR(EditorScene);

public:
    // GameObject
    void Update() override;
    void OnResize(int newWidth, int newHeight) override;

    Scene *GetOpenScene() const;

    void OpenTab(GameObject *tabbedChild);
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
    AnimatorSMEditor *GetAnimatorSMEditor() const;
    SceneOpenerSaver *GetSceneOpenerSaver() const;
    EditorFileTracker *GetEditorFileTracker() const;
    UISceneEditContainer *GetSceneEditContainer() const;
    UIScenePlayContainer *GetScenePlayContainer() const;
    EditSceneGameObjects *GetEditSceneGameObjects() const;

protected:
    EditorScene();
    virtual ~EditorScene() override;

private:
    ScenePlayer *m_scenePlayer                   = nullptr;
    ProjectManager *m_projectManager             = nullptr;
    EditorClipboard *m_editorClipboard           = nullptr;
    UndoRedoManager *m_undoRedoManager           = nullptr;
    SceneOpenerSaver *m_sceneOpenerSaver         = nullptr;
    EditorFileTracker *m_editorFileTracker       = nullptr;
    EditSceneGameObjects *m_editSceneGameObjects = nullptr;

    Scene *p_openScene = nullptr;

    MenuBar *m_menuBar           = nullptr;
    GameObject *m_mainEditorVLGo = nullptr;

    UITabStation *p_tabStation                 = nullptr;
    Console *p_console                         = nullptr;
    Explorer *p_explorer                       = nullptr;
    Inspector *p_inspector                     = nullptr;
    Hierarchy *p_hierarchy                     = nullptr;
    AnimatorSMEditor *p_animatorSMEditor       = nullptr;
    UISceneEditContainer *p_sceneEditContainer = nullptr;
    UIScenePlayContainer *p_scenePlayContainer = nullptr;

    void Init();

    void BindOpenScene();
    void UnBindOpenScene();

    void SetOpenScene(Scene *openScene);

    // IEventsTabHeader
    void OnTabHeaderClicked(UITabHeader *header) override;

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
