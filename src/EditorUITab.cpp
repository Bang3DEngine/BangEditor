#include "BangEditor/EditorUITab.h"

#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIRendererCacher.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorUITab::EditorUITab(const String &title)
{
    SetName(title);

    GameObjectFactory::CreateUIGameObjectInto(this);
    UIRendererCacher *rendCacher = GameObjectFactory::CreateUIRendererCacherInto(this);
    p_container = rendCacher->GetContainer();
    rendCacher->SetCachingEnabled(true);

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    UILayoutElement *hlLE = AddComponent<UILayoutElement>();
    hlLE->SetFlexibleSize( Vector2::One );

    UIVerticalLayout *mainVL = GetTabContainer()->AddComponent<UIVerticalLayout>();
    mainVL->SetPaddings(5);
    mainVL->SetSpacing(1);

    UILayoutElement *vlLE = GetTabContainer()->AddComponent<UILayoutElement>();
    vlLE->SetFlexibleSize( Vector2(1) );

    p_layoutElement = GetTabContainer()->AddComponent<UILayoutElement>();
    p_layoutElement->SetFlexibleSize(Vector2(1));

    GameObject *titleCont = GameObjectFactory::CreateUIGameObject();
    p_titleText = titleCont->AddComponent<UITextRenderer>();
    p_titleText->SetTextSize(16);
    p_titleText->SetContent(title);

    UILayoutElement *titleLE = titleCont->AddComponent<UILayoutElement>();
    titleLE->SetMinHeight(16);

    p_container->SetParent(this);
    titleCont->SetParent(GetTabContainer());
    GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Min, 10)->SetParent(GetTabContainer());
}

EditorUITab::~EditorUITab()
{

}

GameObject *EditorUITab::GetTabContainer() const
{
    return p_container;
}

UILayoutElement *EditorUITab::GetLayoutElement() const
{
    return p_layoutElement;
}

UITextRenderer *EditorUITab::GetTitleText() const
{
    return p_titleText;
}
