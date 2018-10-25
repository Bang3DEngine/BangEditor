#ifndef ASMLAYERINPUT_H
#define ASMLAYERINPUT_H

#include "Bang/Bang.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsValueChanged.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class UIInputText;
class UILabel;
}

using namespace Bang;
namespace BangEditor
{
class ASMLayerInput : public GameObject,
                      public EventEmitter<IEventsValueChanged>,
                      public EventListener<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR(ASMLayerInput);

public:
    ASMLayerInput();
    virtual ~ASMLayerInput() override;

private:
    UILabel *p_layerTopNameLabel = nullptr;
    UIInputText *p_layerNameInput = nullptr;
    UIInputText *p_boneNameInput = nullptr;

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;

    // Serializable
    virtual void ImportMeta(const MetaNode &metaNode) override;
    virtual void ExportMeta(MetaNode *metaNode) const override;
};
}

#endif  // ASMLAYERINPUT_H
