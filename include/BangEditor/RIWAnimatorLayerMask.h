#ifndef RIWANIMATORLAYERMASK_H
#define RIWANIMATORLAYERMASK_H

#include "Bang/AnimatorLayerMask.h"
#include "Bang/Bang.h"
#include "Bang/UIInputText.h"
#include "BangEditor/RIWResource.h"

using namespace Bang;
namespace BangEditor
{
class UIInputArray;

class RIWAnimatorLayerMask : public RIWResource<AnimatorLayerMask>
{
    GAMEOBJECT_EDITOR(RIWAnimatorLayerMask);

public:
    // InspectorWidget
    void Init() override;

private:
    Array<String> m_previousMaskBoneNames;
    UIInputArray *p_boneNamesInput = nullptr;

    RIWAnimatorLayerMask();
    virtual ~RIWAnimatorLayerMask() override;

    // RIWResource
    Array<String> GetBoneNamesFromInputs() const;
    void UpdateInputsFromResource() override;
    Texture2D *GetIconTexture() const override;
    void OnValueChangedRIWResource(
        EventEmitter<IEventsValueChanged> *object) override;

    friend class ResourceInspectorWidgetFactory;
};

class BoneNameRow : public GameObject,
                    public EventListener<IEventsValueChanged>,
                    public EventEmitter<IEventsValueChanged>
{
public:
    BoneNameRow();

    void SetBoneName(const String &boneName);
    String GetBoneName() const;

    // IEventsValueChanged
    void OnValueChanged(EventEmitter<IEventsValueChanged> *) override;

    // Serializable
    virtual void ImportMeta(const MetaNode &metaNode) override;
    virtual void ExportMeta(MetaNode *metaNode) const override;

private:
    UIInputText *p_boneNameInput = nullptr;
};
}

#endif  // RIWANIMATORLAYERMASK_H
