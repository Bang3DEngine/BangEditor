#include "BangEditor/UITabStation.h"

#include "Bang/TextureFactory.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIDirLayoutMovableSeparator.h"

#include "BangEditor/UITabContainer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UITabStation::UITabStation()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIHorizontalLayout *mainHL = AddComponent<UIHorizontalLayout>();
    mainHL->SetChildrenVerticalStretch(Stretch::FULL);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleSize(Vector2::One);

    p_tabContainerGo = GameObjectFactory::CreateUIGameObject();
    p_tabContainerGo->AddComponent<UIVerticalLayout>();
    auto contLE = p_tabContainerGo->AddComponent<UILayoutElement>();
    contLE->SetFlexibleSize(Vector2::One);

    p_leftTabStationGo = GameObjectFactory::CreateUIGameObject();
    p_leftTabStationGo->AddComponent<UIVerticalLayout>();
    p_leftTabStationGo->SetEnabled(false);
    auto leftLE = p_leftTabStationGo->AddComponent<UILayoutElement>();
    leftLE->SetFlexibleSize(Vector2::One);

    static int x = 0;
    if (x == 0)
    {
        SetName("HEHE");
        p_leftTabStationGo->SetName("LOL");
    }

    p_rightTabStationGo = GameObjectFactory::CreateUIGameObject();
    p_rightTabStationGo->AddComponent<UIVerticalLayout>();
    p_rightTabStationGo->SetEnabled(false);
    auto rightLE = p_rightTabStationGo->AddComponent<UILayoutElement>();
    rightLE->SetFlexibleSize(Vector2::One);

    p_topTabStationGo   = GameObjectFactory::CreateUIGameObject();
    p_topTabStationGo->AddComponent<UIVerticalLayout>();
    p_topTabStationGo->SetEnabled(false);
    auto topLE = p_topTabStationGo->AddComponent<UILayoutElement>();
    topLE->SetFlexibleSize(Vector2::One);

    p_botTabStationGo   = GameObjectFactory::CreateUIGameObject();
    p_botTabStationGo->AddComponent<UIVerticalLayout>();
    p_botTabStationGo->SetEnabled(false);
    auto botLE = p_botTabStationGo->AddComponent<UILayoutElement>();
    botLE->SetFlexibleSize(Vector2::One);

    p_tabContainer = GameObject::Create<UITabContainer>();

    p_leftTabStationGo->SetParent(this);
    p_leftSeparator = GameObjectFactory::CreateUIDirLayoutMovableHSeparator();
    p_leftSeparator->GetGameObject()->SetEnabled(false);
    p_leftSeparator->GetGameObject()->SetParent(this);

    p_tabContainerGo->SetParent(this);

    p_rightSeparator = GameObjectFactory::CreateUIDirLayoutMovableHSeparator();
    p_rightSeparator->GetGameObject()->SetEnabled(false);
    p_rightSeparator->GetGameObject()->SetParent(this);
    p_rightTabStationGo->SetParent(this);

    p_topTabStationGo->SetParent(p_tabContainerGo);
    p_topSeparator = GameObjectFactory::CreateUIDirLayoutMovableVSeparator();
    p_topSeparator->GetGameObject()->SetEnabled(false);
    p_topSeparator->GetGameObject()->SetParent(p_tabContainerGo);

    p_tabContainer->SetParent(p_tabContainerGo);

    p_botSeparator = GameObjectFactory::CreateUIDirLayoutMovableVSeparator();
    p_botSeparator->GetGameObject()->SetEnabled(false);
    p_botSeparator->GetGameObject()->SetParent(p_tabContainerGo);
    p_botTabStationGo->SetParent(p_tabContainerGo);
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
    UITabStation **tabStationPtr = nullptr;
    GameObject *tabStationGo = nullptr;
    UIDirLayoutMovableSeparator *separator = nullptr;
    switch (side)
    {
        case Side::LEFT:
            tabStationPtr = &p_leftTabStation;
            tabStationGo = p_leftTabStationGo;
            separator = p_leftSeparator;
        break;

        case Side::RIGHT:
            tabStationPtr = &p_rightTabStation;
            tabStationGo = p_rightTabStationGo;
            separator = p_rightSeparator;
        break;

        case Side::TOP:
            tabStationPtr = &p_topTabStation;
            tabStationGo = p_topTabStationGo;
            separator = p_topSeparator;
        break;

        case Side::BOT:
            tabStationPtr = &p_botTabStation;
            tabStationGo = p_botTabStationGo;
            separator = p_botSeparator;
        break;
    }

    ASSERT(tabStationPtr)
    if (!*tabStationPtr)
    {
        *tabStationPtr = GameObject::Create<UITabStation>();
        UITabStation *tabStation = *tabStationPtr;
        separator->GetGameObject()->SetEnabled(true);
        tabStationGo->SetEnabled(true);
        tabStation->SetParent(tabStationGo);
    }

    return *tabStationPtr;
}

UITabStation *UITabStation::GetChildStation(Side side) const
{
    switch (side)
    {
        case Side::LEFT:  return p_leftTabStation;
        case Side::RIGHT: return p_rightTabStation;
        case Side::TOP:   return p_topTabStation;
        case Side::BOT:   return p_botTabStation;
    }
    return nullptr;
}

