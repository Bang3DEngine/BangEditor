#ifndef UITABSTATION_H
#define UITABSTATION_H

#include "Bang/Bang.h"
#include "Bang/Alignment.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsDragDrop.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIDirLayoutMovableSeparator;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UITabContainer;

class UITabStation : public GameObject,
                     public EventListener<IEventsDestroy>,
                     public EventListener<IEventsDragDrop>
{
    GAMEOBJECT_EDITOR(UITabStation);

public:
	UITabStation();
    virtual ~UITabStation();

    UITabContainer *GetTabContainer() const;

    UITabStation *GetChildStation(Side side) const;
    UITabStation *FindTabStationOf(GameObject *gameObject);
    UITabStation *FindTabStationOfContainer(UITabContainer *tabContainer);
    UITabStation *GetChildStationAndCreateIfNeeded(Side side);
    void RemoveChildStation(Side side);

private:
    GameObject *p_dragMarker = nullptr;

    GameObject *p_tabContainerGo = nullptr;
    UITabContainer *p_tabContainer = nullptr;

    bool m_dragPutInThisTabContainer = false;
    int  m_dragPutInThisTabContainerTabIndex = 0;
    bool m_dragPutInSideTabContainer = false;
    Side m_dragPutInSideTabContainerSide = Side::LEFT;

    std::array<UIDirLayoutMovableSeparator*, 4> p_separators;
    std::array<GameObject*, 4> p_tabStationGos;
    std::array<UITabStation*, 4> p_tabStations;

    void CleanUpEmptyChildStations();

    // IEventsDragDrop
    virtual void OnDragStarted(EventEmitter<IEventsDragDrop>
                               *dragDropEmitter) override;
    virtual void OnDragUpdate(EventEmitter<IEventsDragDrop>
                              *dragDropEmitter) override;
    virtual void OnDrop(EventEmitter<IEventsDragDrop> *dragDropEmitter,
                        bool inside) override;

    // IEventsDestroy
    virtual void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UITABSTATION_H

