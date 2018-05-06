#include "BangEditor/InspectorWidget.h"

#include "Bang/Paths.h"
#include "Bang/UILabel.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIContentSizeFitter.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

InspectorWidget::InspectorWidget()
{
    SetName("InspectorWidget");
}

InspectorWidget::~InspectorWidget()
{
}

void InspectorWidget::Init()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    p_bgRenderer = AddComponent<UIImageRenderer>();
    p_bgRenderer->SetImageTexture( Resources::Load<Texture2D>(
                                        EPATH("Images/RRect_9s.png")).Get() );
    p_bgRenderer->SetMode(UIImageRenderer::Mode::SLICE_9);
    p_bgRenderer->SetTint(Color::Zero);

    UIVerticalLayout *mainVL = AddComponent<UIVerticalLayout>();
    mainVL->SetPaddings(5);
    mainVL->SetSpacing(1);

    UILayoutElement *vlLE = AddComponent<UILayoutElement>();
    vlLE->SetFlexibleSize( Vector2(1) );

    GameObject *titleGo = CreateTitleGameObject();

    GameObject *topSeparator =
            GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Min, 5, 1.0f);

    GameObject *widgetsGo = GameObjectFactory::CreateUIGameObject();
    UIVerticalLayout *widgetsVL = widgetsGo->AddComponent<UIVerticalLayout>();
    widgetsVL->SetChildrenHorizontalStretch(Stretch::Full);
    widgetsVL->SetPaddings(4);
    widgetsVL->SetSpacing(3);
    UILayoutElement *widgetsLE = widgetsGo->AddComponent<UILayoutElement>();
    widgetsLE->SetFlexibleWidth(1.0f);

    p_widgetsContainer = widgetsGo;

    SetLabelsWidth( DefaultLabelWidth );

    titleGo->SetParent(this);
    topSeparator->SetParent(this);
    widgetsGo->SetParent(this);

    IEventListener::SetReceiveEvents(false);
    InitInnerWidgets();
    IEventListener::SetReceiveEvents(true);
}

void InspectorWidget::InitInnerWidgets()
{
}

void InspectorWidget::SetBackgroundColor(const Color &bgColor)
{
    p_bgRenderer->SetTint(bgColor);
}

void InspectorWidget::SetTitle(const String &title)
{
    p_titleText->SetContent(title);
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

void InspectorWidget::Update()
{
    GameObject::Update();

    IEventListener::SetReceiveEvents(false);
    UpdateFromReference();
    IEventListener::SetReceiveEvents(true);
}

void InspectorWidget::UpdateFromReference()
{
}

void InspectorWidget::AddLabel(const String &content, int height, int width)
{
    UILabel *label = InspectorWidget::CreateWidgetLabel(content, height, width);
    label->GetGameObject()->SetParent( GetWidgetsContainer() );
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
        label = InspectorWidget::CreateWidgetLabel(labelContent, height,
                                                   GetLabelsWidth());
    }

    if (height >= 0)
    {
        UILayoutElement *widgetContLE = widgetContainer->AddComponent<UILayoutElement>();
        widgetContLE->SetPreferredHeight( height );
        widgetContLE->SetFlexibleSize( Vector2::One );
        widgetContLE->SetLayoutPriority(1);
    }

    widgetContainer->SetParent( GetWidgetsContainer() );
    if (label)
    {
        label->GetGameObject()->SetParent( widgetContainer );
    }
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

UILabel *InspectorWidget::CreateWidgetLabel(const String &content,
                                            int _height,
                                            int _width)
{
    int height = _height >= 0 ? _height : DefaultWidgetHeight;
    int width  = _width >= 0 ? _width : GetLabelsWidth();

    UILabel *label = GameObjectFactory::CreateUILabel();
    label->GetText()->SetContent(content);
    label->GetText()->SetTextSize(12);
    label->GetText()->SetHorizontalAlign(HorizontalAlignment::Left);
    label->SetSelectable(false);

    UILayoutElement *labelLE =
                  label->GetGameObject()->GetComponent<UILayoutElement>();
    labelLE->SetPreferredWidth( width );
    labelLE->SetPreferredHeight( height );
    labelLE->SetFlexibleWidth(0);

    m_labelToLabelLE.Add(label, labelLE);

    return label;
}

int InspectorWidget::GetLabelsWidth() const
{
    return m_labelsWidth;
}

GameObject *InspectorWidget::GetWidgetsContainer() const
{
    return p_widgetsContainer;
}

GameObject *InspectorWidget::CreateTitleGameObject()
{
    GameObject *titleGo = GameObjectFactory::CreateUIGameObject();
    UITextRenderer *titleText = titleGo->AddComponent<UITextRenderer>();
    titleText->SetHorizontalAlign(HorizontalAlignment::Left);
    titleText->SetContent("InspectorWidget");
    titleText->SetTextSize(13);
    p_titleText = titleText;

    return titleGo;
}
