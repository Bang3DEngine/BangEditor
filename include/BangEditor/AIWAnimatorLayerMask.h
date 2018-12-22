#ifndef AIWANIMATORLAYERMASK_H
#define AIWANIMATORLAYERMASK_H

#include "Bang/AnimatorLayerMask.h"
#include "Bang/Bang.h"
#include "Bang/UIInputText.h"
#include "BangEditor/AIWAsset.h"

using namespace Bang;
namespace BangEditor
{
class UIInputArray;

class AIWAnimatorLayerMask : public AIWAsset<AnimatorLayerMask>
{
    GAMEOBJECT_EDITOR(AIWAnimatorLayerMask);

public:
    // InspectorWidget
    void Init() override;

private:
    Array<String> m_previousMaskBoneNames;
    UIInputArray *p_boneNamesInput = nullptr;

    AIWAnimatorLayerMask();
    virtual ~AIWAnimatorLayerMask() override;

    // AIWAsset
    Array<AnimatorLayerMask::BoneEntry> GetBoneEntriesFromInputs() const;
    void UpdateInputsFromAsset() override;
    Texture2D *GetIconTexture() const override;
    void OnValueChangedAIWAsset(
        EventEmitter<IEventsValueChanged> *object) override;

    friend class AssetInspectorWidgetFactory;
};

class BoneEntryRow : public GameObject,
                     public EventListener<IEventsValueChanged>,
                     public EventEmitter<IEventsValueChanged>
{
public:
    BoneEntryRow();

    void SetBoneName(const String &boneName);
    void SetAddAscendants(bool addAscendants);
    void SetAddDescendants(bool addDescendants);

    String GetBoneName() const;
    bool GetAddAscendants() const;
    bool GetAddDescendants() const;

    // IEventsValueChanged
    void OnValueChanged(EventEmitter<IEventsValueChanged> *) override;

    // Serializable
    virtual void ImportMeta(const MetaNode &metaNode) override;
    virtual void ExportMeta(MetaNode *metaNode) const override;

private:
    UIInputText *p_boneNameInput = nullptr;
    UICheckBox *p_addAscendantsInput = nullptr;
    UICheckBox *p_addDescendantsInput = nullptr;
};
}

#endif  // AIWANIMATORLAYERMASK_H
