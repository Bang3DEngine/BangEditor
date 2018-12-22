#include "BangEditor/RIWAnimatorLayerMask.h"

#include "Bang/GameObjectFactory.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIInputText.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/UIInputArray.h"

using namespace Bang;
using namespace BangEditor;

RIWAnimatorLayerMask::RIWAnimatorLayerMask()
{
}

RIWAnimatorLayerMask::~RIWAnimatorLayerMask()
{
}

void RIWAnimatorLayerMask::Init()
{
    RIWResource<AnimatorLayerMask>::Init();

    SetName("RIWAnimatorLayerMask");
    SetTitle("Animator Layer Mask");

    p_boneNamesInput = new UIInputArray();
    p_boneNamesInput->SetUpdateFunctions<BoneEntryRow>([this]() {
        BoneEntryRow *boneNameRow = new BoneEntryRow();
        boneNameRow->EventEmitter<IEventsValueChanged>::RegisterListener(this);
        return boneNameRow;
    });
    p_boneNamesInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddLabel("Mask bone names");
    // AddWidget(
    //     GameObjectFactory::CreateUIHSeparator(LayoutSizeType::PREFERRED, 5));
    AddWidget(p_boneNamesInput);

    SetLabelsWidth(100);
}

void RIWAnimatorLayerMask::UpdateInputsFromResource()
{
    Array<AnimatorLayerMask::BoneEntry> layerBoneMaskEntries =
        GetResource().Get()->GetBoneEntries();
    if (layerBoneMaskEntries != GetBoneEntriesFromInputs())
    {
        p_boneNamesInput->Clear();

        for (const AnimatorLayerMask::BoneEntry &boneEntry :
             layerBoneMaskEntries)
        {
            BoneEntryRow *boneNameRow = new BoneEntryRow();
            boneNameRow->SetBoneName(boneEntry.boneName);
            boneNameRow->SetAddDescendants(boneEntry.addDescendants);
            boneNameRow->SetAddAscendants(boneEntry.addAscendants);
            boneNameRow->EventEmitter<IEventsValueChanged>::RegisterListener(
                this);
            p_boneNamesInput->AddRow(boneNameRow);
        }
    }
}

Texture2D *RIWAnimatorLayerMask::GetIconTexture() const
{
    return EditorTextureFactory::GetAnimatorLayerMaskIcon();
}

void RIWAnimatorLayerMask::OnValueChangedRIWResource(
    EventEmitter<IEventsValueChanged> *object)
{
    BANG_UNUSED(object);

    AnimatorLayerMask *layerMask = (GetResource().Get());
    layerMask->ClearBoneEntries();
    Array<AnimatorLayerMask::BoneEntry> maskBoneEntries =
        GetBoneEntriesFromInputs();
    for (const AnimatorLayerMask::BoneEntry &maskBoneEntry : maskBoneEntries)
    {
        layerMask->AddBoneEntry(maskBoneEntry);
    }
}

Array<AnimatorLayerMask::BoneEntry>
RIWAnimatorLayerMask::GetBoneEntriesFromInputs() const
{
    Array<AnimatorLayerMask::BoneEntry> maskBoneEntries;
    Array<GameObject *> boneNameRowGos = p_boneNamesInput->GetRowGameObjects();
    for (GameObject *boneNameRowGo : boneNameRowGos)
    {
        BoneEntryRow *boneEntryRow = DCAST<BoneEntryRow *>(boneNameRowGo);
        ASSERT(boneEntryRow);

        AnimatorLayerMask::BoneEntry boneEntry;
        boneEntry.boneName = boneEntryRow->GetBoneName();
        boneEntry.addDescendants = boneEntryRow->GetAddDescendants();
        boneEntry.addAscendants = boneEntryRow->GetAddAscendants();

        maskBoneEntries.PushBack(boneEntry);
    }
    return maskBoneEntries;
}

// BoneNameRow

BoneEntryRow::BoneEntryRow()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetSpacing(5);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinHeight(20);
    le->SetFlexibleWidth(1.0f);

    p_boneNameInput = GameObjectFactory::CreateUIInputText();
    p_boneNameInput->GetGameObject()->SetParent(this);
    p_boneNameInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_addAscendantsInput = GameObjectFactory::CreateUICheckBox();
    p_addAscendantsInput->GetGameObject()->SetParent(this);
    p_addAscendantsInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_addDescendantsInput = GameObjectFactory::CreateUICheckBox();
    p_addDescendantsInput->GetGameObject()->SetParent(this);
    p_addDescendantsInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
}

void BoneEntryRow::SetBoneName(const String &boneName)
{
    p_boneNameInput->GetText()->SetContent(boneName);
}

void BoneEntryRow::SetAddAscendants(bool addAscendants)
{
    p_addAscendantsInput->SetChecked(addAscendants);
}

void BoneEntryRow::SetAddDescendants(bool addDescendants)
{
    p_addDescendantsInput->SetChecked(addDescendants);
}

String BoneEntryRow::GetBoneName() const
{
    return p_boneNameInput->GetText()->GetContent();
}

bool BoneEntryRow::GetAddAscendants() const
{
    return p_addAscendantsInput->IsChecked();
}

bool BoneEntryRow::GetAddDescendants() const
{
    return p_addDescendantsInput->IsChecked();
}

void BoneEntryRow::OnValueChanged(EventEmitter<IEventsValueChanged> *)
{
    EventEmitter<IEventsValueChanged>::PropagateToListeners(
        &IEventsValueChanged::OnValueChanged, this);
}

void BoneEntryRow::ImportMeta(const MetaNode &metaNode)
{
    if (metaNode.Contains("BoneName"))
    {
        String boneName = metaNode.Get<String>("BoneName");
        p_boneNameInput->GetText()->SetContent(boneName);
    }
    if (metaNode.Contains("AddDescendants"))
    {
        bool addDescendants = metaNode.Get<bool>("AddDescendants");
        p_addDescendantsInput->SetChecked(addDescendants);
    }
    if (metaNode.Contains("AddAscendants"))
    {
        bool addAscendants = metaNode.Get<bool>("AddAscendants");
        p_addAscendantsInput->SetChecked(addAscendants);
    }
}

void BoneEntryRow::ExportMeta(MetaNode *metaNode) const
{
    metaNode->Set("BoneName", p_boneNameInput->GetText()->GetContent());
    metaNode->Set("AddDescendants", p_addDescendantsInput->IsChecked());
    metaNode->Set("AddAscendants", p_addAscendantsInput->IsChecked());
}
