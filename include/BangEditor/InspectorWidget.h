#ifndef INSPECTORWIDGET_H
#define INSPECTORWIDGET_H

#include "Bang/BangDefines.h"
#include "Bang/EventListener.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsDestroy.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/List.h"
#include "Bang/String.h"
#include "Bang/UMap.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class Color;
class UICheckBox;
class UIImageRenderer;
class UILabel;
class UILayoutElement;
class UITextRenderer;
}  // namespace Bang

using namespace Bang;
namespace BangEditor
{
class InspectorWidgetTitle;

class InspectorWidget : public GameObject,
                        public EventListener<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(InspectorWidget);

public:
    InspectorWidget();

    virtual void Init();

    void AddLabel(const String &content, int height = -1, int width = -1);
    void AddWidget(GameObject *widget, int height = DefaultWidgetHeight);
    void AddWidget(const String &labelContent,
                   GameObject *widget,
                   int height = DefaultWidgetHeight);
    void RemoveWidget(GameObject *widget, bool destroy);

    const String &GetTitle() const;
    void SetBackgroundColor(const Color &bgColor);
    InspectorWidgetTitle *GetInspectorWidgetTitle() const;
    const UMap<GameObject *, UILabel *> &GetWidgetToLabel() const;

protected:
    static constexpr int DefaultLabelWidth = 50;
    static constexpr int DefaultWidgetHeight = 20;

    virtual ~InspectorWidget() override;

    virtual void InitInnerWidgets();

    virtual void SetTitle(const String &title);
    void SetLabelsWidth(int labelsWidth);

    // GameObject
    virtual void Update() override;

    virtual void UpdateFromReference();

    void SetWidgetEnabled(GameObject *widget, bool enabled);

    int GetLabelsWidth() const;
    GameObject *GetWidgetFromLabel(const String &labelStr) const;
    GameObject *GetWidgetsContainer() const;

    virtual InspectorWidgetTitle *CreateTitleGameObject();

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;

private:
    int m_labelsWidth = -1;
    List<GameObject *> p_widgets;
    UMap<GameObject *, UILabel *> m_widgetToLabel;
    UMap<UILabel *, UILayoutElement *> m_labelToLabelLE;

    InspectorWidgetTitle *p_inspectorWidgetTitleGo = nullptr;
    UIImageRenderer *p_bgRenderer = nullptr;
    GameObject *p_widgetsContainer = nullptr;

    UILabel *CreateWidgetLabel(const String &content, int height, int width);

    void AddWidgetInternal(const String &labelContent,
                           GameObject *widget,
                           int height,
                           bool addLabel);
};

// Title GameObject for an InspectorWidget
class InspectorWidgetTitle : public GameObject
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(InspectorWidgetTitle);

public:
    InspectorWidgetTitle();
    virtual ~InspectorWidgetTitle() override;

    UITextRenderer *GetText() const;
    UIImageRenderer *GetIcon() const;
    UITextRenderer *GetEnabledText() const;
    UICheckBox *GetEnabledCheckBox() const;

private:
    UITextRenderer *p_text = nullptr;
    UIImageRenderer *p_icon = nullptr;
    UITextRenderer *p_enabledText = nullptr;
    UICheckBox *p_enabledCheckBox = nullptr;
};
}  // namespace BangEditor

#endif  // INSPECTORWIDGET_H
