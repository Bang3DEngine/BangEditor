#ifndef UITABSTATION_H
#define UITABSTATION_H

#include "Bang/Bang.h"
#include "Bang/Alignment.h"
#include "Bang/GameObject.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIDirLayoutMovableSeparator;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UITabContainer;

class UITabStation : public GameObject
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

private:
    GameObject *p_tabContainerGo = nullptr;
    UITabContainer *p_tabContainer = nullptr;

    UIDirLayoutMovableSeparator *p_topSeparator   = nullptr;
    UIDirLayoutMovableSeparator *p_botSeparator   = nullptr;
    UIDirLayoutMovableSeparator *p_leftSeparator  = nullptr;
    UIDirLayoutMovableSeparator *p_rightSeparator = nullptr;

    GameObject *p_leftTabStationGo  = nullptr;
    GameObject *p_rightTabStationGo = nullptr;
    GameObject *p_topTabStationGo   = nullptr;
    GameObject *p_botTabStationGo   = nullptr;

    UITabStation *p_leftTabStation  = nullptr;
    UITabStation *p_rightTabStation = nullptr;
    UITabStation *p_topTabStation   = nullptr;
    UITabStation *p_botTabStation   = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // UITABSTATION_H

