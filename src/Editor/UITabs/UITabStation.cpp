#include "BangEditor/UITabStation.h"

#include "Bang/Array.h"
#include "Bang/Assert.h"
#include "Bang/Color.h"
#include "Bang/Component.h"
#include "Bang/DPtr.h"
#include "Bang/DPtr.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsDragDrop.h"
#include "Bang/Input.h"
#include "Bang/Math.h"
#include "Bang/RectTransform.h"
#include "Bang/Stretch.h"
#include "Bang/UIDirLayoutMovableSeparator.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UITheme.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/Vector.tcc"
#include "Bang/Vector2.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/UITabContainer.h"
#include "BangEditor/UITabHeader.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class IEventsDestroy;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UITabStation::UITabStation()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIHorizontalLayout *mainHL = AddComponent<UIHorizontalLayout>();
    mainHL->SetChildrenVerticalStretch(Stretch::FULL);

    p_dragMarker = GameObjectFactory::CreateUIGameObject();
    UIImageRenderer *dragMarkerImg = p_dragMarker->AddComponent<UIImageRenderer>();
    dragMarkerImg->SetTint( UITheme::GetSelectedColor().WithAlpha(0.75f) );
    p_dragMarker->SetEnabled(false);
    p_dragMarker->GetRectTransform()->SetLocalPosition( Vector3(0, 0, -0.01f) );
    p_dragMarker->SetParent( EditorSceneManager::GetEditorScene() );

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleSize(Vector2::One);

    p_tabContainerGo = GameObjectFactory::CreateUIGameObject();
    p_tabContainerGo->AddComponent<UIVerticalLayout>();
    auto contLE = p_tabContainerGo->AddComponent<UILayoutElement>();
    contLE->SetFlexibleSize(Vector2::One);

    for (int s = 0; s < 4; ++s)
    {
        p_tabStations[s] = nullptr;

        p_tabStationGos[s] = GameObjectFactory::CreateUIGameObject();
        p_tabStationGos[s]->AddComponent<UIVerticalLayout>();
        p_tabStationGos[s]->SetEnabled(false);
        auto stationGoLE = p_tabStationGos[s]->AddComponent<UILayoutElement>();
        stationGoLE->SetFlexibleSize(Vector2::One);

        Side side = SCAST<Side>(s);
        p_separators[s] = (side == Side::LEFT || side == Side::RIGHT) ?
                    GameObjectFactory::CreateUIDirLayoutMovableHSeparator() :
                    GameObjectFactory::CreateUIDirLayoutMovableVSeparator();
        p_separators[s]->GetGameObject()->SetEnabled(false);
    }

    p_tabContainer = GameObject::Create<UITabContainer>();

    p_tabStationGos[ SCAST<int>(Side::LEFT) ]->SetParent(this);
    p_separators[ SCAST<int>(Side::LEFT) ]->GetGameObject()->SetParent(this);

    p_tabContainerGo->SetParent(this);

    p_separators[ SCAST<int>(Side::RIGHT) ]->GetGameObject()->SetParent(this);
    p_tabStationGos[ SCAST<int>(Side::RIGHT) ]->SetParent(this);

    p_tabStationGos[ SCAST<int>(Side::TOP) ]->SetParent(p_tabContainerGo);
    p_separators[ SCAST<int>(Side::TOP) ]->GetGameObject()->
                                           SetParent(p_tabContainerGo);

    p_tabContainer->SetParent(p_tabContainerGo);

    p_separators[ SCAST<int>(Side::BOT) ]->GetGameObject()->
                                           SetParent(p_tabContainerGo);
    p_tabStationGos[ SCAST<int>(Side::BOT) ]->SetParent(p_tabContainerGo);
}

UITabStation::~UITabStation()
{
}

UITabContainer *UITabStation::GetTabContainer() const
{
    return p_tabContainer;
}

UITabStation *UITabStation::GetChildStationAndCreateIfNeeded(Side side)
{
    int s = SCAST<int>(side);
    if (!p_tabStations[s])
    {
        p_tabStations[s] = GameObject::Create<UITabStation>();
        UITabStation *tabStation = p_tabStations[s];
        p_separators[s]->GetGameObject()->SetEnabled(true);
        p_tabStationGos[s]->SetEnabled(true);
        tabStation->SetParent( p_tabStationGos[s] );
    }
    return p_tabStations[s];
}

void UITabStation::RemoveChildStation(Side side)
{
    if (UITabStation *station = GetChildStation(side))
    {
        GameObject::Destroy(station);

        int s = SCAST<int>(side);
        p_tabStations[s] = nullptr;
        p_separators[s]->GetGameObject()->SetEnabled(false);
        p_tabStationGos[s]->SetEnabled(false);
    }
}

void UITabStation::CleanUpEmptyChildStations()
{
    for (int s = 0; s < 4; ++s)
    {
        Side side = SCAST<Side>(s);
        if (UITabStation *sideStt = GetChildStation(side))
        {
            UITabContainer *sideTabCont = sideStt->GetTabContainer();
            if (sideTabCont->GetTabbedChildren().Size() == 0 &&
                !sideStt->GetChildStation(Side::LEFT)  &&
                !sideStt->GetChildStation(Side::RIGHT) &&
                !sideStt->GetChildStation(Side::TOP)   &&
                !sideStt->GetChildStation(Side::BOT))
            {
                RemoveChildStation(side);
            }
        }
    }
}

void UITabStation::OnDragStarted(EventEmitter<IEventsDragDrop> *dragDropEmitter)
{
    if (Component *comp = DCAST<Component*>(dragDropEmitter))
    {
        if (UITabHeader *tabHeader = DCAST<UITabHeader*>(comp->GetGameObject()))
        {
            if (UITabContainer *tabContainer = tabHeader->GetTabContainer())
            {
                tabContainer->RemoveTabByHeader(tabHeader, false);
                CleanUpEmptyChildStations();
            }
        }
    }
}

void UITabStation::OnDragUpdate(EventEmitter<IEventsDragDrop> *dragDropEmitter)
{
    if (Component *comp = DCAST<Component*>(dragDropEmitter))
    {
        if (UITabHeader *draggedTabHeader =
                            DCAST<UITabHeader*>(comp->GetGameObject()))
        {
            BANG_UNUSED(draggedTabHeader);
            if (GetTabContainer()->IsWaitingToBeDestroyed())
            {
                return;
            }

            Vector2 mousePos( Input::GetMousePosition() );

            UITabHeader *prevTabHeader = nullptr;
            UITabHeader *nextTabHeader = nullptr;
            m_dragPutInThisTabContainer = false;
            m_dragPutInSideTabContainer = false;

            p_dragMarker->SetEnabled(false);
            GameObject *headersBar = GetTabContainer()->GetHeadersBar();
            bool overHeadersBar = (headersBar->GetRectTransform()->IsMouseOver());
            if (overHeadersBar)
            {
                m_dragPutInThisTabContainer = true;
                float closestDistToPrevTH = Math::Infinity<float>();
                float closestDistToNextTH = Math::Infinity<float>();
                auto thisTabHeaders = GetTabContainer()->GetTabHeaders();
                for (UITabHeader *tabHeader : thisTabHeaders)
                {
                    RectTransform *tabHeaderRT = tabHeader->GetRectTransform();
                    AARect tabHeaderRect = tabHeaderRT->GetViewportAARect();
                    Vector2 thCenter = tabHeaderRect.GetCenter();
                    float distToTH = Math::Abs(thCenter.x - mousePos.x);
                    if (distToTH < closestDistToPrevTH &&
                        mousePos.x < thCenter.x)
                    {
                        nextTabHeader = tabHeader;
                        closestDistToPrevTH = distToTH;
                    }
                    else if (distToTH < closestDistToNextTH &&
                             mousePos.x >= thCenter.x)
                    {
                        prevTabHeader = tabHeader;
                        closestDistToNextTH = distToTH;
                    }
                }

                m_dragPutInThisTabContainerTabIndex =
                        SCAST<int>(thisTabHeaders.IndexOf(prevTabHeader)) + 1;
                ASSERT(m_dragPutInThisTabContainerTabIndex >= 0 &&
                       m_dragPutInThisTabContainerTabIndex <= thisTabHeaders.Size());
            }
            else if (GetRectTransform()->IsMouseOver())
            {
                UITabStation *leftSt  = GetChildStation(Side::LEFT);
                UITabStation *rightSt = GetChildStation(Side::RIGHT);
                UITabStation *topSt   = GetChildStation(Side::TOP);
                UITabStation *botSt   = GetChildStation(Side::BOT);
                bool isOverTerminalStation =
                   (!leftSt  ||  !leftSt->GetRectTransform()->IsMouseOver()) &&
                   (!rightSt || !rightSt->GetRectTransform()->IsMouseOver()) &&
                   (!topSt   ||   !topSt->GetRectTransform()->IsMouseOver()) &&
                   (!botSt   ||   !botSt->GetRectTransform()->IsMouseOver());

                if (isOverTerminalStation)
                {
                    AARect stationRect = GetTabContainer()->GetRectTransform()->
                                         GetViewportAARect();
                    Vector2 stationCenter = stationRect.GetCenter();
                    Vector2 stationSize = stationRect.GetSize();
                    if (!leftSt &&
                        mousePos.x < stationCenter.x - stationSize.x / 4)
                    {
                        m_dragPutInSideTabContainer = true;
                        m_dragPutInSideTabContainerSide = Side::LEFT;
                    }
                    else if (!rightSt &&
                             mousePos.x >= stationCenter.x + stationSize.x / 4)
                    {
                        m_dragPutInSideTabContainer = true;
                        m_dragPutInSideTabContainerSide = Side::RIGHT;
                    }
                    else if (!botSt &&
                             mousePos.y < stationCenter.y - stationSize.y / 4)
                    {
                        m_dragPutInSideTabContainer = true;
                        m_dragPutInSideTabContainerSide = Side::BOT;
                    }
                    else if (!topSt &&
                             mousePos.y >= stationCenter.y + stationSize.y / 4)
                    {
                        m_dragPutInSideTabContainer = true;
                        m_dragPutInSideTabContainerSide = Side::TOP;
                    }
                    else
                    {
                        m_dragPutInThisTabContainer = true;
                        m_dragPutInSideTabContainer = false;
                    }
                }
            }

            if (m_dragPutInThisTabContainer)
            {
                if (UITabHeader *tabHeaderToMark = prevTabHeader ? prevTabHeader :
                                                                   nextTabHeader)
                {
                    bool markingPrev = (tabHeaderToMark == prevTabHeader);
                    float markX = markingPrev ? 1 : -1;
                    RectTransform *dragMarkerRT = p_dragMarker->GetRectTransform();
                    dragMarkerRT->SetAnchors( Vector2(markX, -1), Vector2(markX, 1) );
                    dragMarkerRT->SetMarginLeftBot(  Vector2i(-3, 0) );
                    dragMarkerRT->SetMarginRightTop( Vector2i(-3, 0) );
                    p_dragMarker->SetParent(tabHeaderToMark);
                }
                else
                {
                    p_dragMarker->GetRectTransform()->SetAnchors(-Vector2::One,
                                                                  Vector2::One);
                    p_dragMarker->SetParent( GetTabContainer() );
                }
                p_dragMarker->SetEnabled(true);
            }
            else if (m_dragPutInSideTabContainer)
            {
                UITabContainer *dragMarkerParent = GetTabContainer();
                Vector2 markerAnchorMin;
                Vector2 markerAnchorMax;
                switch (m_dragPutInSideTabContainerSide)
                {
                    case Side::LEFT:
                        markerAnchorMin = Vector2(  -1, -1);
                        markerAnchorMax = Vector2(-0.5,  1);
                    break;

                    case Side::RIGHT:
                        markerAnchorMin = Vector2(0.5, -1);
                        markerAnchorMax = Vector2(  1,  1);
                    break;

                    case Side::BOT:
                        markerAnchorMin = Vector2(-1,   -1);
                        markerAnchorMax = Vector2( 1, -0.5);
                    break;

                    case Side::TOP:
                        markerAnchorMin = Vector2(-1, 0.5);
                        markerAnchorMax = Vector2( 1,   1);
                    break;
                }

                RectTransform *dragMarkerRT = p_dragMarker->GetRectTransform();
                dragMarkerRT->SetAnchorMin(markerAnchorMin);
                dragMarkerRT->SetAnchorMax(markerAnchorMax);
                p_dragMarker->SetParent(dragMarkerParent);
                p_dragMarker->SetEnabled(true);
            }
        }
    }
}

void UITabStation::OnDrop(EventEmitter<IEventsDragDrop> *dragDropEmitter)
{
    BANG_UNUSED(dragDropEmitter);
    if (Component *comp = DCAST<Component*>(dragDropEmitter))
    {
        if (UITabHeader *draggedTabHeader =
                            DCAST<UITabHeader*>(comp->GetGameObject()))
        {
            p_dragMarker->SetEnabled(false);
            if (m_dragPutInThisTabContainer)
            {
                draggedTabHeader->p_dragTabContainerDestiny = GetTabContainer();
                draggedTabHeader->m_dragTabContainerDestinyIndex =
                                            m_dragPutInThisTabContainerTabIndex;
            }
            else if (m_dragPutInSideTabContainer)
            {
                draggedTabHeader->p_dragTabContainerDestiny =
                        GetChildStationAndCreateIfNeeded(
                            m_dragPutInSideTabContainerSide)->GetTabContainer();
                draggedTabHeader->m_dragTabContainerDestinyIndex = -1u;
            }

            m_dragPutInThisTabContainer = false;
            m_dragPutInSideTabContainer = false;

            draggedTabHeader->OnDrop(dragDropEmitter);

            CleanUpEmptyChildStations();
        }
    }
}

void UITabStation::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    GameObject::OnDestroyed(object);
}

UITabStation *UITabStation::GetChildStation(Side side) const
{
    return p_tabStations[ SCAST<int>(side) ];
}

UITabStation *UITabStation::FindTabStationOf(GameObject *gameObject)
{
    const auto &childrenInTabs = GetTabContainer()->GetTabbedChildren();
    for (GameObject *childInTabs : childrenInTabs)
    {
        if (childInTabs == gameObject)
        {
            return this;
        }
    }

    for (int sidei = 0; sidei < 4; ++sidei)
    {
        Side side = SCAST<Side>(sidei);
        if (GetChildStation(side))
        {
            if (UITabStation *tabStation =
                GetChildStation(side)->FindTabStationOf(gameObject))
            {
                return tabStation;
            }
        }
    }

    return nullptr;
}

UITabStation *UITabStation::FindTabStationOfContainer(UITabContainer *tabContainer)
{
    if (tabContainer == GetTabContainer())
    {
        return this;
    }

    for (int s = 0; s < 4; ++s)
    {
        Side side = SCAST<Side>(s);
        if (GetChildStation(side))
        {
            if (UITabStation *tabStation =
                GetChildStation(side)->FindTabStationOfContainer(tabContainer))
            {
                return tabStation;
            }
        }
    }

    return nullptr;
}

