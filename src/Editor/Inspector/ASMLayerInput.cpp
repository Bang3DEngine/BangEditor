#include "BangEditor/ASMLayerInput.h"

#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIInputText.h"
#include "Bang/UILabel.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIVerticalLayout.h"

using namespace Bang;
using namespace BangEditor;

ASMLayerInput::ASMLayerInput()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();
    vl->SetSpacing(5);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinHeight(80);
    le->SetFlexibleSize(Vector2(1.0f));

    p_layerTopNameLabel = GameObjectFactory::CreateUILabel();
    p_layerTopNameLabel->GetText()->SetContent("Label");
    p_layerTopNameLabel->GetText()->SetTextSize(15);
    p_layerTopNameLabel->GetText()->SetHorizontalAlign(
        HorizontalAlignment::LEFT);
    p_layerTopNameLabel->GetText()->SetVerticalAlign(VerticalAlignment::TOP);
    p_layerTopNameLabel->GetGameObject()
        ->GetComponent<UILayoutElement>()
        ->SetFlexibleSize(Vector2(1, 1));
    p_layerTopNameLabel->GetGameObject()->SetParent(this);

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
    layerNameRow->SetParent(this);

    GameObject *boneNameRow = GameObjectFactory::CreateUIGameObject();
    {
        UIHorizontalLayout *rowHL =
            boneNameRow->AddComponent<UIHorizontalLayout>();
        rowHL->SetSpacing(5);

        UILabel *label = GameObjectFactory::CreateUILabel();
        label->GetText()->SetContent("Bone name:");
        label->GetText()->SetHorizontalAlign(HorizontalAlignment::LEFT);
        label->GetGameObject()->SetParent(boneNameRow);
        UILayoutElement *labelLE =
            label->GetGameObject()->AddComponent<UILayoutElement>();
        labelLE->SetMinSize(Vector2i(100, 20));
        labelLE->SetPreferredWidth(labelLE->GetMinWidth());
        labelLE->SetFlexibleSize(Vector2(0, 1));
        labelLE->SetLayoutPriority(2);

        p_boneNameInput = GameObjectFactory::CreateUIInputText();
        p_boneNameInput->GetText()->SetContent("BoneName");
        p_boneNameInput->EventEmitter<IEventsValueChanged>::RegisterListener(
            this);
        p_boneNameInput->GetGameObject()->SetParent(boneNameRow);
    }
    boneNameRow->SetParent(this);

    GameObjectFactory::CreateUIHSeparator(LayoutSizeType::MIN, 10)
        ->SetParent(this);
}

ASMLayerInput::~ASMLayerInput()
{
}

void ASMLayerInput::OnValueChanged(EventEmitter<IEventsValueChanged> *ee)
{
    if (ee == p_layerNameInput)
    {
    }

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

    if (metaNode.Contains("BoneName"))
    {
        String boneName = metaNode.Get<String>("BoneName");
        p_boneNameInput->GetText()->SetContent(boneName);
    }
}

void ASMLayerInput::ExportMeta(MetaNode *metaNode) const
{
    Serializable::ExportMeta(metaNode);

    metaNode->Set("LayerName", p_layerNameInput->GetText()->GetContent());
    metaNode->Set("BoneName", p_boneNameInput->GetText()->GetContent());
}
