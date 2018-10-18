#include "BangEditor/InspectorWidget.h"

#include "Bang/Alignment.h"
#include "Bang/Assert.h"
#include "Bang/Color.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/List.tcc"
#include "Bang/Stretch.h"
#include "Bang/TextureFactory.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILabel.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/UMap.tcc"

using namespace Bang;
using namespace BangEditor;

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
    // p_bgRenderer->SetImageTexture(
    // TextureFactory::Get9SliceRoundRectTexture().Get() );
    // p_bgRenderer->SetMode(UIImageRenderer::Mode::SLICE_9);
    p_bgRenderer->SetTint(Color::Zero);

    GameObjectFactory::AddInnerBorder(this);

    UIVerticalLayout *mainVL = AddComponent<UIVerticalLayout>();
    mainVL->SetSpacing(1);

    UILayoutElement *vlLE = AddComponent<UILayoutElement>();
    vlLE->SetFlexibleSize(Vector2(1));

    p_inspectorWidgetTitleGo = CreateTitleGameObject();

    GameObject *topSpacer =
        GameObjectFactory::CreateUIVSpacer(LayoutSizeType::MIN, 4);

    GameObject *widgetsGo = GameObjectFactory::CreateUIGameObject();
    UIVerticalLayout *widgetsVL = widgetsGo->AddComponent<UIVerticalLayout>();
    widgetsVL->SetChildrenHorizontalStretch(Stretch::FULL);
    widgetsVL->SetPaddings(10);
    widgetsVL->SetSpacing(3);
    UILayoutElement *widgetsLE = widgetsGo->AddComponent<UILayoutElement>();
    widgetsLE->SetFlexibleWidth(1.0f);

    p_widgetsContainer = widgetsGo;

    SetLabelsWidth(DefaultLabelWidth);

    p_inspectorWidgetTitleGo->SetParent(this);
    topSpacer->SetParent(this);
    widgetsGo->SetParent(this);

    SetReceiveEventsCommon(false);
    InitInnerWidgets();
    SetReceiveEventsCommon(true);

    GameObjectFactory::AddOuterShadow(this, Vector2i(3), 0.4f);
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
    p_inspectorWidgetTitleGo->GetText()->SetContent(title);
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
                labelLE->SetPreferredWidth(GetLabelsWidth());
            }
        }
    }
}

void InspectorWidget::Update()
{
    GameObject::Update();

    SetReceiveEventsCommon(false);
    UpdateFromReference();
    SetReceiveEventsCommon(true);
}

void InspectorWidget::UpdateFromReference()
{
}

void InspectorWidget::AddLabel(const String &content, int height, int width)
{
    UILabel *label = InspectorWidget::CreateWidgetLabel(content, height, width);
    label->GetGameObject()->SetParent(GetWidgetsContainer());
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

const String &InspectorWidget::GetTitle() const
{
    return p_inspectorWidgetTitleGo->GetText()->GetContent();
}

void InspectorWidget::AddWidgetInternal(const String &labelContent,
                                        GameObject *widget,
                                        int height,
                                        bool addLabel)
{
    GameObject *widgetContainer = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *widgetContHL =
        widgetContainer->AddComponent<UIHorizontalLayout>();
    widgetContHL->SetChildrenVerticalStretch(Stretch::FULL);

    UILabel *label = nullptr;
    if (addLabel)
    {
        label = InspectorWidget::CreateWidgetLabel(
            labelContent, height, GetLabelsWidth());
        label->GetFocusable()->SetEnabled(false);
    }

    if (height >= 0)
    {
        UILayoutElement *widgetContLE =
            widgetContainer->AddComponent<UILayoutElement>();
        widgetContLE->SetPreferredHeight(height);
        widgetContLE->SetFlexibleSize(Vector2::One);
        widgetContLE->SetLayoutPriority(1);
    }

    widgetContainer->SetParent(GetWidgetsContainer());
    if (label)
    {
        label->GetGameObject()->SetParent(widgetContainer);
    }
    GameObjectFactory::CreateUIHSpacer(LayoutSizeType::FLEXIBLE, 0.0001f)
        ->SetParent(widgetContainer);
    widget->SetParent(widgetContainer);

    m_widgetToLabel.Add(widget, label);
    p_widgets.PushBack(widget);
}

void InspectorWidget::RemoveWidget(GameObject *widget)
{
    GameObject *widgetContainer = widget->GetParent();
    p_widgets.Remove(widget);
    m_widgetToLabel.Remove(widget);
    GameObject::Destroy(widgetContainer);
}

void InspectorWidget::SetWidgetEnabled(GameObject *widget, bool enabled)
{
    widget->GetParent()->SetEnabled(enabled);
}

UILabel *InspectorWidget::CreateWidgetLabel(const String &content,
                                            int _height,
                                            int _width)
{
    int height = _height >= 0 ? _height : DefaultWidgetHeight;
    int width = _width >= 0 ? _width : GetLabelsWidth();

    UILabel *label = GameObjectFactory::CreateUILabel();
    label->GetText()->SetContent(content);
    label->GetText()->SetTextSize(12);
    label->GetText()->SetHorizontalAlign(HorizontalAlignment::LEFT);
    label->SetSelectable(false);

    UILayoutElement *labelLE =
        label->GetGameObject()->AddComponent<UILayoutElement>();
    labelLE->SetMinWidth(width);
    labelLE->SetPreferredWidth(width);
    labelLE->SetPreferredHeight(height);
    labelLE->SetFlexibleWidth(0);
    labelLE->SetLayoutPriority(2);

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

InspectorWidgetTitle *InspectorWidget::CreateTitleGameObject()
{
    InspectorWidgetTitle *widgetTitleGo =
        new InspectorWidgetTitle();
    return widgetTitleGo;
}

InspectorWidgetTitle *InspectorWidget::GetInspectorWidgetTitle() const
{
    return p_inspectorWidgetTitleGo;
}

void InspectorWidget::OnValueChanged(EventEmitter<IEventsValueChanged> *ee)
{
    BANG_UNUSED(ee);
}

InspectorWidgetTitle::InspectorWidgetTitle()
{
    GameObjectFactory::CreateUIGameObjectInto(this);
    UIHorizontalLayout *titleHL = AddComponent<UIHorizontalLayout>();
    titleHL->SetPaddings(5, 7, 5, 5);
    titleHL->SetSpacing(5);

    UIImageRenderer *bg = AddComponent<UIImageRenderer>();
    bg->SetImageTexture(TextureFactory::GetButtonIdle());
    bg->SetMode(UIImageRenderer::Mode::SLICE_9_INV_UVY);
    bg->SetTint(Color::White.WithValue(1.1f));

    GameObjectFactory::AddInnerBorder(this);

    p_icon = GameObjectFactory::CreateUIImage(Color::White);
    GameObject *iconGo = p_icon->GetGameObject();
    UILayoutElement *iconLE = iconGo->AddComponent<UILayoutElement>();
    iconLE->SetPreferredSize(Vector2i(16));
    p_icon->SetTint(Color::DarkGray);

    GameObject *titleTextGo = GameObjectFactory::CreateUIGameObject();
    UITextRenderer *titleText = titleTextGo->AddComponent<UITextRenderer>();
    titleText->SetHorizontalAlign(HorizontalAlignment::LEFT);
    titleText->SetContent("InspectorWidget");
    titleText->SetTextSize(12);
    p_text = titleText;

    UILayoutElement *titleLE = titleTextGo->AddComponent<UILayoutElement>();
    titleLE->SetFlexibleWidth(99.9f);

    GameObject *enabledTextGo = GameObjectFactory::CreateUIGameObject();
    UITextRenderer *enabledText = enabledTextGo->AddComponent<UITextRenderer>();
    enabledText->SetContent("Enabled");
    enabledText->SetTextSize(12);
    enabledTextGo->SetEnabled(false);
    p_enabledText = enabledText;

    p_enabledCheckBox = GameObjectFactory::CreateUICheckBox();
    GameObject *enabledCheckBoxGo = p_enabledCheckBox->GetGameObject();

    iconGo->SetParent(this);
    titleTextGo->SetParent(this);
    enabledTextGo->SetParent(this);
    enabledCheckBoxGo->SetParent(this);
}

InspectorWidgetTitle::~InspectorWidgetTitle()
{
}

UITextRenderer *InspectorWidgetTitle::GetText() const
{
    return p_text;
}

UIImageRenderer *InspectorWidgetTitle::GetIcon() const
{
    return p_icon;
}

UITextRenderer *InspectorWidgetTitle::GetEnabledText() const
{
    return p_enabledText;
}

UICheckBox *InspectorWidgetTitle::GetEnabledCheckBox() const
{
    return p_enabledCheckBox;
}
