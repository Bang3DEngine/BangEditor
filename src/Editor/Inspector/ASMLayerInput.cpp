#include "BangEditor/ASMLayerInput.h"

#include "Bang/AnimatorLayerMask.h"
#include "Bang/AnimatorStateMachineLayer.h"
#include "Bang/Extensions.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/MetaFilesManager.h"
#include "Bang/Assets.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIInputText.h"
#include "Bang/UILabel.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UITheme.h"
#include "Bang/UIVerticalLayout.h"
#include "BangEditor/UIInputFile.h"

using namespace Bang;
using namespace BangEditor;

ASMLayerInput::ASMLayerInput()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    p_focusable = AddComponent<UIFocusable>();
    p_focusable->SetConsiderForTabbing(false);
    p_focusable->EventEmitter<IEventsFocus>::RegisterListener(this);

    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();
    vl->SetSpacing(5);
    vl->SetPaddings(5);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinHeight(100);
    le->SetFlexibleSize(Vector2(1.0f));

    GameObject *contentGo = GameObjectFactory::CreateUIGameObject();
    contentGo->SetParent(this);

    UIVerticalLayout *contentVL = contentGo->AddComponent<UIVerticalLayout>();
    contentVL->SetSpacing(5);
    contentVL->SetPaddings(5);

    p_focusBg = contentGo->AddComponent<UIImageRenderer>();
    p_focusBg->SetTint(Color::Zero());

    UILayoutElement *contentLE = contentGo->AddComponent<UILayoutElement>();
    contentLE->SetFlexibleSize(Vector2(1.0f));

    GameObject *labelHLGo = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *labelHL = labelHLGo->AddComponent<UIHorizontalLayout>();
    labelHL->SetSpacing(10);
    labelHLGo->SetParent(contentGo);

    p_layerTopNameLabel = GameObjectFactory::CreateUILabel();
    p_layerTopNameLabel->GetText()->SetContent("Label");
    p_layerTopNameLabel->GetText()->SetTextSize(15);
    p_layerTopNameLabel->GetText()->SetHorizontalAlign(
        HorizontalAlignment::LEFT);
    p_layerTopNameLabel->GetText()->SetVerticalAlign(VerticalAlignment::TOP);
    p_layerTopNameLabel->GetGameObject()
        ->GetComponent<UILayoutElement>()
        ->SetFlexibleSize(Vector2(1, 1));
    p_layerTopNameLabel->GetGameObject()->SetParent(labelHLGo);

    GameObjectFactory::CreateUIHSpacer(LayoutSizeType::FLEXIBLE, 9999.9f)
        ->SetParent(labelHLGo);

    UILabel *enabledLabel = GameObjectFactory::CreateUILabel();
    enabledLabel->GetText()->SetContent("Enabled");
    enabledLabel->GetGameObject()->SetParent(labelHLGo);

    p_layerEnabledInput = GameObjectFactory::CreateUICheckBox();
    p_layerEnabledInput->GetGameObject()->SetParent(labelHLGo);
    p_layerEnabledInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    GameObject *layerNameRow = GameObjectFactory::CreateUIGameObject();
    {
        UIHorizontalLayout *rowHL =
            layerNameRow->AddComponent<UIHorizontalLayout>();
        rowHL->SetSpacing(5);

        UILabel *label = GameObjectFactory::CreateUILabel();
        label->GetText()->SetContent("Layer name:");
        label->GetText()->SetHorizontalAlign(HorizontalAlignment::LEFT);
        label->GetGameObject()->SetParent(layerNameRow);
        UILayoutElement *labelLE =
            label->GetGameObject()->AddComponent<UILayoutElement>();
        labelLE->SetMinSize(Vector2i(100, 20));
        labelLE->SetPreferredWidth(labelLE->GetMinWidth());
        labelLE->SetFlexibleSize(Vector2(0, 1));
        labelLE->SetLayoutPriority(2);

        p_layerNameInput = GameObjectFactory::CreateUIInputText();
        p_layerNameInput->GetText()->SetContent("LayerName");
        p_layerNameInput->EventEmitter<IEventsValueChanged>::RegisterListener(
            this);
        p_layerNameInput->GetGameObject()->SetParent(layerNameRow);
    }
    layerNameRow->SetParent(contentGo);

    GameObject *layerMaskRow = GameObjectFactory::CreateUIGameObject();
    {
        UIHorizontalLayout *rowHL =
            layerMaskRow->AddComponent<UIHorizontalLayout>();
        rowHL->SetSpacing(5);

        UILabel *label = GameObjectFactory::CreateUILabel();
        label->GetText()->SetContent("Mask:");
        label->GetText()->SetHorizontalAlign(HorizontalAlignment::LEFT);
        label->GetGameObject()->SetParent(layerMaskRow);
        UILayoutElement *labelLE =
            label->GetGameObject()->AddComponent<UILayoutElement>();
        labelLE->SetMinSize(Vector2i(100, 20));
        labelLE->SetPreferredWidth(labelLE->GetMinWidth());
        labelLE->SetFlexibleSize(Vector2(0, 1));
        labelLE->SetLayoutPriority(2);

        p_layerMaskInput = new UIInputFile();
        p_layerMaskInput->SetExtensions(
            {Extensions::GetAnimatorLayerMaskExtension()});
        p_layerMaskInput->SetZoomable(false);
        p_layerMaskInput->EventEmitter<IEventsValueChanged>::RegisterListener(
            this);
        p_layerMaskInput->SetParent(layerMaskRow);
    }
    layerMaskRow->SetParent(contentGo);

    GameObjectFactory::CreateUIHSeparator(LayoutSizeType::MIN, 10)
        ->SetParent(this);
}

ASMLayerInput::~ASMLayerInput()
{
}

void ASMLayerInput::Update()
{
    GameObject::Update();
}

void ASMLayerInput::Select()
{
    m_selected = true;
    p_focusBg->SetTint(UITheme::GetSelectedColor());

    EventEmitter<IEventsASMLayerInput>::PropagateToListeners(
        &IEventsASMLayerInput::OnLayerInputSelected, this);
}

void ASMLayerInput::UnSelect()
{
    m_selected = false;
    p_focusBg->SetTint(Color::Zero());
}

bool ASMLayerInput::IsSelected() const
{
    return m_selected;
}

UIEventResult ASMLayerInput::OnUIEvent(UIFocusable *focusable,
                                       const UIEvent &event)
{
    BANG_UNUSED(focusable);
    switch (event.type)
    {
        case UIEvent::Type::MOUSE_CLICK_FULL: Select(); break;

        default: break;
    }

    return UIEventResult::IGNORE;
}

void ASMLayerInput::OnValueChanged(EventEmitter<IEventsValueChanged> *)
{
    EventEmitter<IEventsValueChanged>::PropagateToListeners(
        &IEventsValueChanged::OnValueChanged, this);
}

void ASMLayerInput::ImportMeta(const MetaNode &metaNode)
{
    Serializable::ImportMeta(metaNode);

    if (metaNode.Contains("LayerName"))
    {
        String layerName = metaNode.Get<String>("LayerName");
        p_layerNameInput->GetText()->SetContent(layerName);
        p_layerTopNameLabel->GetText()->SetContent(layerName);
    }

    if (metaNode.Contains("Enabled"))
    {
        p_layerEnabledInput->SetChecked(metaNode.Get<bool>("Enabled"));
    }

    if (metaNode.Contains("LayerMask"))
    {
        AH<AnimatorLayerMask> layerMask =
            Assets::Load<AnimatorLayerMask>(metaNode.Get<GUID>("LayerMask"));
        p_layerMaskInput->SetPath(layerMask.Get()
                                      ? layerMask.Get()->GetAssetFilepath()
                                      : Path::Empty());
    }

    m_layerNodesMetas = metaNode.GetChildren("Nodes");
}

void ASMLayerInput::ExportMeta(MetaNode *metaNode) const
{
    Serializable::ExportMeta(metaNode);

    metaNode->Set("LayerName", p_layerNameInput->GetText()->GetContent());
    metaNode->Set("Enabled", p_layerEnabledInput->IsChecked());
    metaNode->Set("LayerMask",
                  MetaFilesManager::GetGUID(p_layerMaskInput->GetPath()));

    for (const MetaNode &nodeMeta : m_layerNodesMetas)
    {
        metaNode->AddChild(nodeMeta, "Nodes");
    }
}
