#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsComponent.h"
#include "Bang/IEventsDestroy.h"
#include "Bang/IEventsFileTracker.h"
#include "Bang/List.h"
#include "Bang/Path.h"
#include "Bang/SceneManager.h"
#include "Bang/String.h"
#include "Bang/UMap.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/Editor.h"

namespace Bang
{
class Component;
class Object;
class Scene;
class Serializable;
class UIImageRenderer;
class UITextRenderer;
class UIScrollPanel;
class UIVerticalLayout;
template <class>
class EventEmitter;
}  // namespace Bang

using namespace Bang;
namespace BangEditor
{
class IEventsEditor;
class InspectorWidget;
class MenuItem;
class UIContextMenu;

class Inspector : public GameObject,
                  public EventListener<IEventsDestroy>,
                  public EventListener<IEventsEditor>,
                  public EventListener<IEventsComponent>,
                  public EventListener<IEventsFileTracker>,
                  public EventListener<IEventsSceneManager>
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(Inspector);

public:
    Inspector();
    virtual ~Inspector() override;

    // GameObject
    void Update() override;

    void ShowSerializable(Serializable *serializable);
    void ShowPath(const Path &path);
    void ShowGameObject(GameObject *gameObject);
    void ShowInspectorWidget(InspectorWidget *inspectorWidget);

    void Clear();

    const Path &GetCurrentPath() const;
    GameObject *GetCurrentGameObject() const;

    static Inspector *GetActive();

private:
    List<InspectorWidget *> m_widgets;
    UMap<Object *, InspectorWidget *> m_objToWidget;

    Path m_currentOpenPath = Path::Empty();
    GameObject *p_currentGameObject = nullptr;
    UIVerticalLayout *p_mainVL = nullptr;
    UIScrollPanel *p_scrollPanel = nullptr;
    UITextRenderer *p_titleText = nullptr;
    UIContextMenu *p_contextMenu = nullptr;
    UIImageRenderer *p_blockLayer = nullptr;

    GameObject *GetWidgetsContainer() const;
    UIVerticalLayout *GetMainVL() const;
    UIScrollPanel *GetScrollPanel() const;

    void AddWidget(InspectorWidget *widget, int index = -1);
    void RemoveWidget(InspectorWidget *widget);
    void RemoveWidget(int index);
    void SetCurrentWidgetBlocked(bool blocked);

    // UIContextMenu
    virtual void OnCreateContextMenu(MenuItem *menuRootItem);

    // IEventsSceneManager
    void OnSceneLoaded(Scene *scene, const Path &sceneFilepath) override;

    // IEventsDestroy
    void OnDestroyed(EventEmitter<IEventsDestroy> *destroyedObject) override;

    // IFileTrackerListener
    void OnPathRemoved(const Path &removedPath) override;

    // IEditorListener
    void OnExplorerPathSelected(const Path &path) override;
    void OnGameObjectSelected(GameObject *selectedGameObject) override;

    // IEventsComponent
    void OnComponentAdded(Component *addedComponent, int index) override;
    void OnComponentRemoved(Component *removedComponent,
                            GameObject *previousGameObject) override;
};
}  // namespace BangEditor

#endif  // INSPECTOR_H
