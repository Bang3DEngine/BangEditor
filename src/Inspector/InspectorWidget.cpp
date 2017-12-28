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

    p_widgetsContainer = widgetsGo;
    p_title = titleText;

    SetLabelsWidth(50);

    titleGo->SetParent(this);
    topSeparator->SetParent(this);
    widgetsGo->SetParent(this);
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

void InspectorWidget::SetLabelsWidth(int labelsWidth)
{
    if (GetLabelsWidth() != labelsWidth)
    {
        m_labelsWidth = labelsWidth;

        for (GameObject *widget : p_widgets)
        {
            ASSERT(m_widgetToLabel.ContainsKey(widget))
            UILabel *label = m_widgetToLabel[widget];
            if (label)
            {
                ASSERT(m_labelToLabelLE.ContainsKey(label));
                UILayoutElement *labelLE = m_labelToLabelLE[label];
                labelLE->SetPreferredWidth( GetLabelsWidth() );
            }
        }
    }
}

void InspectorWidget::AddWidget(GameObject *widget, int height)
{
    AddWidgetInternal("", widget, height, false);
}

void InspectorWidget::AddWidget(const String &labelContent,
                                GameObject *widget,
                                int height)
{
    AddWidgetInternal(labelContent, widget, height, true);
}

void InspectorWidget::AddWidgetInternal(const String &labelContent,
                                        GameObject *widget,
                                        int height,
                                        bool addLabel)
{
    GameObject *widgetContainer = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *widgetContHL = widgetContainer->AddComponent<UIHorizontalLayout>();
    widgetContHL->SetChildrenVerticalStretch(Stretch::Full);

    UILabel *label = nullptr;
    if (addLabel)
    {
        label = GameObjectFactory::CreateUILabel();
        label->GetText()->SetContent(labelContent);
        label->GetText()->SetTextSize(11);
        label->GetText()->SetHorizontalAlign(HorizontalAlignment::Left);
        label->SetSelectable(false);

        UILayoutElement *labelLE =
                      label->GetGameObject()->GetComponent<UILayoutElement>();
        labelLE->SetPreferredWidth( GetLabelsWidth() );
        labelLE->SetPreferredHeight( height );
        labelLE->SetFlexibleWidth(0);

        m_labelToLabelLE.Add(label, labelLE);
    }

    UILayoutElement *widgetContLE = widgetContainer->AddComponent<UILayoutElement>();
    widgetContLE->SetPreferredHeight( height );
    widgetContLE->SetFlexibleSize( Vector2::One );
    widgetContLE->SetLayoutPriority(1);

    widgetContainer->SetParent( GetWidgetsContainer() );
    if (addLabel) { label->GetGameObject()->SetParent( widgetContainer ); }
    GameObjectFactory::CreateUIHSpacer(LayoutSizeType::Flexible, 0.0001f)
                       ->SetParent( widgetContainer );
    widget->SetParent( widgetContainer );

    m_widgetToLabel.Add(widget, label);
    p_widgets.PushBack(widget);
}


void InspectorWidget::SetWidgetEnabled(GameObject *widget, bool enabled)
{
    widget->GetParent()->SetEnabled( enabled );
}

int InspectorWidget::GetLabelsWidth() const
{
    return m_labelsWidth;
}

GameObject *InspectorWidget::GetWidgetsContainer() const
{
    return p_widgetsContainer;
}
