#ifndef ASMLAYERINPUT_H
#define ASMLAYERINPUT_H

#include "Bang/Bang.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsFocus.h"
#include "Bang/IEventsValueChanged.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class AnimatorStateMachineLayer;
class UICheckBox;
class UIFocusable;
class UIInputText;
class UILabel;
class UIImageRenderer;
}

using namespace Bang;
namespace BangEditor
{
class ASMLayerInput;
class UIInputFile;

class IEventsASMLayerInput
{
    IEVENTS(IEventsASMLayerInput);

public:
    virtual void OnLayerInputSelected(ASMLayerInput *selectedLayerInput) = 0;
};

class ASMLayerInput : public GameObject,
                      public EventEmitter<IEventsValueChanged>,
                      public EventListener<IEventsValueChanged>,
                      public EventListener<IEventsFocus>,
                      public EventEmitter<IEventsASMLayerInput>
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(ASMLayerInput);

public:
    ASMLayerInput();
    virtual ~ASMLayerInput() override;

    // GameObject
    void Update() override;

    void Select();
    void UnSelect();
    bool IsSelected() const;

private:
    bool m_selected = false;

    Array<MetaNode> m_layerNodesMetas;
    UIFocusable *p_focusable = nullptr;
    UIImageRenderer *p_focusBg = nullptr;
    UILabel *p_layerTopNameLabel = nullptr;
    UICheckBox *p_layerEnabledInput = nullptr;
    UIInputText *p_layerNameInput = nullptr;
    UIInputFile *p_layerMaskInput = nullptr;

    // IEventsFocus
    UIEventResult OnUIEvent(UIFocusable *focusable,
                            const UIEvent &event) override;

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;

    // Serializable
    virtual void ImportMeta(const MetaNode &metaNode) override;
    virtual void ExportMeta(MetaNode *metaNode) const override;
};
}

#endif  // ASMLAYERINPUT_H
