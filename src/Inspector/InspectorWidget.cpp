#include "BangEditor/InspectorWidget.h"

#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

InspectorWidget::InspectorWidget()
{
    SetName("InspectorWidget");
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();
    vl->SetPaddings(4);

    GameObject *titleGo = GameObjectFactory::CreateUIGameObject();

    UITextRenderer *text = titleGo->AddComponent<UITextRenderer>();
    text->SetHorizontalAlign(HorizontalAlignment::Left);
    text->SetContent("InspectorWidget");
    text->SetTextSize(13);

    GameObject *topSeparator =
            GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Min, 5, 1.0f);
    GameObject *botSeparator =
            GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Min, 5, 1.0f);

    GameObject *container = GameObjectFactory::CreateUIGameObject("Container");
    UIVerticalLayout *containerVL = container->AddComponent<UIVerticalLayout>();

    p_container = container;
    p_title = text;

    AddChild(titleGo);
    AddChild(topSeparator);
    AddChild(container);
    AddChild(botSeparator);
}

InspectorWidget::~InspectorWidget()
{
}

void InspectorWidget::SetBackgroundColor(const Color &bgColor)
{
    GetComponent<UIImageRenderer>()->SetTint(bgColor);
}

void InspectorWidget::SetTitle(const String &title)
{
    p_title->SetContent(title);
}
GameObject *InspectorWidget::GetContainer() const { return p_container; }
