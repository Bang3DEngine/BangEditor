#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include <vector>

#include "BangMath/AARect.h"
#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsDestroy.h"
#include "Bang/IEventsFileTracker.h"
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

namespace Bang
{
class GameObject;
class Scene;
class Texture2D;
class UIImageRenderer;
class UITextRenderer;
class Path;
class Window;
template <class>
class EventEmitter;
}  // namespace Bang

using namespace Bang;
namespace BangEditor
{
class AnimatorSMEditor;
class Console;
class EditSceneGameObjects;
class EditorApplication;
class EditorMeshFactoryUITab;
class EditorClipboard;
class EditorFileTracker;
class Explorer;
class Hierarchy;
class IEventsScenePlayer;
class IEventsTabHeader;
class Inspector;
class MenuBar;
class SceneOpenerSaver;
class ScenePlayer;
class UISceneContainer;
class UISceneEditContainer;
class UIScenePlayContainer;
class UITabContainer;
class UITabHeader;
class UITabStation;
class UndoRedoManager;

class EditorScene : public Scene,
                    public EventListener<IEventsWindow>,
                    public EventListener<IEventsTabHeader>,
                    public EventListener<IEventsScenePlayer>,
                    public EventListener<IEventsSceneManager>
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(EditorScene);

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
    EditorClipboard *GetEditorClipboard() const;
    UndoRedoManager *GetUndoRedoManager() const;
    AnimatorSMEditor *GetAnimatorSMEditor() const;
    EditorMeshFactoryUITab *GetEditorMeshFactoryTab() const;
    SceneOpenerSaver *GetSceneOpenerSaver() const;
    EditorFileTracker *GetEditorFileTracker() const;
    UISceneEditContainer *GetSceneEditContainer() const;
    UIScenePlayContainer *GetScenePlayContainer() const;
    EditSceneGameObjects *GetEditSceneGameObjects() const;

protected:
    EditorScene();
    virtual ~EditorScene() override;

private:
    ScenePlayer *m_scenePlayer = nullptr;
    EditorClipboard *m_editorClipboard = nullptr;
    UndoRedoManager *m_undoRedoManager = nullptr;
    SceneOpenerSaver *m_sceneOpenerSaver = nullptr;
    EditorFileTracker *m_editorFileTracker = nullptr;
    EditSceneGameObjects *m_editSceneGameObjects = nullptr;

    Scene *p_openScene = nullptr;

    MenuBar *m_menuBar = nullptr;
    GameObject *m_mainEditorVLGo = nullptr;

    UITabStation *p_tabStation = nullptr;
    Console *p_console = nullptr;
    Explorer *p_explorer = nullptr;
    Inspector *p_inspector = nullptr;
    Hierarchy *p_hierarchy = nullptr;
    AnimatorSMEditor *p_animatorSMEditor = nullptr;
    EditorMeshFactoryUITab *p_editorMeshFactoryTab = nullptr;
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
}  // namespace BangEditor

#endif  // EDITORSCENE_H
