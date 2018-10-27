#include "BangEditor/RIWAnimatorLayerMask.h"

#include "Bang/GameObjectFactory.h"
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
    p_boneNamesInput->SetUpdateFunctions<BoneNameRow>([this]() {
        BoneNameRow *boneNameRow = new BoneNameRow();
        boneNameRow->EventEmitter<IEventsValueChanged>::RegisterListener(this);
        return boneNameRow;
    });
    p_boneNamesInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddLabel("Mask bone names");
    AddWidget(
        GameObjectFactory::CreateUIHSeparator(LayoutSizeType::PREFERRED, 5));
    AddWidget(p_boneNamesInput);

    SetLabelsWidth(100);
}

void RIWAnimatorLayerMask::UpdateInputsFromResource()
{
    Array<String> layerBoneMaskNames = GetResource().Get()->GetBoneMaskNames();
    if (layerBoneMaskNames != GetBoneNamesFromInputs())
    {
        p_boneNamesInput->Clear();

        for (const String &boneName : layerBoneMaskNames)
        {
            BoneNameRow *boneNameRow = new BoneNameRow();
            boneNameRow->SetBoneName(boneName);
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

    GetResource().Get()->ClearBones();
    Array<String> maskBoneNames = GetBoneNamesFromInputs();
    for (const String &maskBoneName : maskBoneNames)
    {
        GetResource().Get()->AddBone(maskBoneName);
    }
}

Array<String> RIWAnimatorLayerMask::GetBoneNamesFromInputs() const
{
    Array<String> maskBoneNames;
    Array<GameObject *> boneNameRowGos = p_boneNamesInput->GetRowGameObjects();
    for (GameObject *boneNameRowGo : boneNameRowGos)
    {
        BoneNameRow *boneNameRow = DCAST<BoneNameRow *>(boneNameRowGo);
        ASSERT(boneNameRow);
        maskBoneNames.PushBack(boneNameRow->GetBoneName());
    }
    return maskBoneNames;
}

// BoneNameRow

BoneNameRow::BoneNameRow()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    AddComponent<UIHorizontalLayout>();

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinHeight(20);
    le->SetFlexibleWidth(1.0f);

    p_boneNameInput = GameObjectFactory::CreateUIInputText();
    p_boneNameInput->GetGameObject()->SetParent(this);
    p_boneNameInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
}

void BoneNameRow::SetBoneName(const String &boneName)
{
    p_boneNameInput->GetText()->SetContent(boneName);
}

String BoneNameRow::GetBoneName() const
{
    return p_boneNameInput->GetText()->GetContent();
}

void BoneNameRow::OnValueChanged(EventEmitter<IEventsValueChanged> *)
{
    EventEmitter<IEventsValueChanged>::PropagateToListeners(
        &IEventsValueChanged::OnValueChanged, this);
}

void BoneNameRow::ImportMeta(const MetaNode &metaNode)
{
    if (metaNode.Contains("BoneName"))
    {
        String boneName = metaNode.Get<String>("BoneName");
        p_boneNameInput->GetText()->SetContent(boneName);
    }
}

void BoneNameRow::ExportMeta(MetaNode *metaNode) const
{
    metaNode->Set("BoneName", p_boneNameInput->GetText()->GetContent());
}
