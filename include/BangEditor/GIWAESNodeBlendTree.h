#ifndef GIWAESNODEBLENDTREE_H
#define GIWAESNODEBLENDTREE_H

#include "Bang/Bang.h"
#include "BangEditor/GIWAESNode.h"

using namespace Bang;
namespace Bang
{
class UIComboBox;
class UIInputNumber;
}

namespace BangEditor
{
class GIWAESNodeBlendTree : public GIWAESNode
{
public:
    GIWAESNodeBlendTree();
    virtual ~GIWAESNodeBlendTree();

    // InspectorWidget
    virtual void InitInnerWidgets() override;

protected:
    UIComboBox *p_variableNameInput = nullptr;
    UIInputNumber *p_secondAnimationSpeedInput = nullptr;
    UIInputFile *p_nodeSecondAnimationInput = nullptr;

    // InspectorWidget
    virtual void UpdateFromReference() override;

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;

    // IEventsDestroy
    virtual void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;
};
}

#endif  // GIWAESNODEBLENDTREE_H
