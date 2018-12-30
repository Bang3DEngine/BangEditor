#ifndef UITABSTATION_H
#define UITABSTATION_H

#include <array>
#include <vector>

#include "Bang/Alignment.h"
#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsDragDrop.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class IEventsDestroy;
class IEventsDragDrop;
class UIDirLayoutMovableSeparator;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UITabContainer;

class UITabStation : public GameObject,
                     public EventListener<IEventsDragDrop>,
                     public EventListener<IEventsDestroy>
{
    GAMEOBJECT_EDITOR(UITabStation);

public:
    UITabStation();
    virtual ~UITabStation() override;

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
    int m_dragPutInThisTabContainerTabIndex = 0;
    bool m_dragPutInSideTabContainer = false;
    Side m_dragPutInSideTabContainerSide = Side::LEFT;

    std::array<UIDirLayoutMovableSeparator *, 4> p_separators;
    std::array<GameObject *, 4> p_tabStationGos;
    std::array<UITabStation *, 4> p_tabStations;

    void CleanUpEmptyChildStations();

    // IEventsDragDrop
    virtual void OnDragStarted(
        EventEmitter<IEventsDragDrop> *dragDropEmitter) override;
    virtual void OnDragUpdate(
        EventEmitter<IEventsDragDrop> *dragDropEmitter) override;
    virtual void OnDrop(
        EventEmitter<IEventsDragDrop> *dragDropEmitter) override;

    // IEventsDestroy
    virtual void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;
};
}

#endif  // UITABSTATION_H
