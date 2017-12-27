#include "BangEditor/InspectorWidget.h"

#include "Bang/UILabel.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

InspectorWidget::InspectorWidget()
{
    SetName("InspectorWidget");
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIVerticalLayout *mainVL = AddComponent<UIVerticalLayout>();
    mainVL->SetPaddings(0);
    mainVL->SetSpacing(0);

    GameObject *titleGo = GameObjectFactory::CreateUIGameObject();
    UITextRenderer *titleText = titleGo->AddComponent<UITextRenderer>();
    titleText->SetHorizontalAlign(HorizontalAlignment::Left);
    titleText->SetContent("InspectorWidget");
    titleText->SetTextSize(10);

    GameObject *topSeparator =
            GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Min, 5, 1.0f);

    GameObject *hlGo = GameObjectFactory::CreateUIGameObject();
    UILayoutElement *hlLE = hlGo->AddComponent<UILayoutElement>();
    hlLE->SetFlexibleSize( Vector2::One );
    UIHorizontalLayout *hl = hlGo->AddComponent<UIHorizontalLayout>();
    hl->SetChildrenVerticalAlignment(VerticalAlignment::Center);
    hl->SetChildrenHorizontalAlignment(HorizontalAlignment::Center);
    hl->SetPaddings(4);
    hl->SetSpacing(3);

    GameObject *labelsGo = GameObjectFactory::CreateUIGameObject();
    UIVerticalLayout *labelsVL = labelsGo->AddComponent<UIVerticalLayout>();
    labelsVL->SetChildrenHorizontalAlignment(HorizontalAlignment::Left);
    labelsVL->SetPaddings(4);
    labelsVL->SetSpacing(3);
    UILayoutElement *labelsLE = labelsGo->AddComponent<UILayoutElement>();
    labelsLE->SetFlexibleSize( Vector2::Zero );

    GameObject *widgetsGo = GameObjectFactory::CreateUIGameObject();
    UIVerticalLayout *widgetsVL = widgetsGo->AddComponent<UIVerticalLayout>();
    widgetsVL->SetChildrenHorizontalStretch(Stretch::Full);
    widgetsVL->SetPaddings(4);
    widgetsVL->SetSpacing(3);
    UILayoutElement *widgetsLE = widgetsGo->AddComponent<UILayoutElement>();
    widgetsLE->SetFlexibleWidth(1.0f);

    GameObject *botSeparator =
            GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Min, 5, 1.0f);
    GameObject *bot2Separator =
            GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Min, 10, 1.1f);

    p_labelsContainer = labelsGo;
    p_widgetsContainer = widgetsGo;
    p_title = titleText;

    titleGo->SetParent(this);
    topSeparator->SetParent(this);
    hlGo->SetParent(this);
    labelsGo->SetParent(hlGo);
    widgetsGo->SetParent(hlGo);
    botSeparator->SetParent(this);
    bot2Separator->SetParent(this);
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

void InspectorWidget::AddWidget(const String &labelContent,
                                GameObject *widget,
                                int height)
{
    UILabel *label = GameObjectFactory::CreateUILabel();
    label->GetText()->SetContent(labelContent);
    label->GetText()->SetTextSize(11);
    label->GetText()->SetHorizontalAlign(HorizontalAlignment::Left);
    label->SetSelectable(false);
    UILayoutElement *labelLE =
                  label->GetGameObject()->AddComponent<UILayoutElement>();
    labelLE->SetPreferredHeight(height);
    labelLE->SetFlexibleWidth(0);

    GameObject *widgetContainer = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *widgetContHL = widgetContainer->AddComponent<UIHorizontalLayout>();
    widgetContHL->SetChildrenVerticalStretch(Stretch::Full);

    UILayoutElement *widgetContLE = widgetContainer->AddComponent<UILayoutElement>();
    widgetContLE->SetPreferredHeight( height );
    widgetContLE->SetFlexibleSize( Vector2::One );
    widgetContLE->SetLayoutPriority(1);

    label->GetGameObject()->SetParent( GetLabelsContainer() );
    widgetContainer->SetParent( GetWidgetsContainer() );
    GameObjectFactory::CreateUIHSpacer(LayoutSizeType::Flexible, 0.0001f)
                       ->SetParent( widgetContainer );
    widget->SetParent( widgetContainer );
}

void InspectorWidget::SetWidgetEnabled(GameObject *widget, bool enabled)
{
    int i = 0;
    for (GameObject *w : GetWidgetsContainer()->GetChildren())
    {
        if (w == widget) { break; }
        ++i;
    }

    GameObject *widgetLabelGo = GetLabelsContainer()->GetChild(i);

    widget->SetEnabled(enabled);
    widgetLabelGo->SetEnabled(enabled);
}

GameObject *InspectorWidget::GetLabelsContainer() const
{
    return p_labelsContainer;
}

GameObject *InspectorWidget::GetWidgetsContainer() const
{
    return p_widgetsContainer;
}
