﻿#ifndef INSPECTOR_H
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

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class Component;
FORWARD   class IEventsComponent;
FORWARD   class IEventsDestroy;
FORWARD   class IEventsFileTracker;
FORWARD   class IEventsSceneManager;
FORWARD   class Object;
FORWARD   class Scene;
FORWARD   class Serializable;
FORWARD   class UIImageRenderer;
FORWARD   class UITextRenderer;
FORWARD   class UIScrollPanel;
FORWARD   class UIVerticalLayout;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class IEventsEditor;
FORWARD class InspectorWidget;
FORWARD class MenuItem;
FORWARD class UIContextMenu;

class Inspector : public GameObject,
                  public EventListener<IEventsEditor>,
                  public EventListener<IEventsComponent>,
                  public EventListener<IEventsFileTracker>,
                  public EventListener<IEventsSceneManager>
{
    GAMEOBJECT_EDITOR(Inspector);

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

    static Inspector* GetActive();

private:
    List<InspectorWidget*> m_widgets;
    UMap<Object*, InspectorWidget*> m_objToWidget;

    Path m_currentOpenPath = Path::Empty;
    GameObject *p_currentGameObject = nullptr;
    UIVerticalLayout *p_mainVL   = nullptr;
    UIScrollPanel *p_scrollPanel = nullptr;
    UITextRenderer *p_titleText = nullptr;
    UIContextMenu *p_contextMenu = nullptr;
    UIImageRenderer *p_blockLayer = nullptr;

    GameObject* GetWidgetsContainer() const;
    UIVerticalLayout *GetMainVL() const;
    UIScrollPanel* GetScrollPanel() const;

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

NAMESPACE_BANG_EDITOR_END

#endif // INSPECTOR_H
