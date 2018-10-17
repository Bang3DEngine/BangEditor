#ifndef GIWAESNODE_H
#define GIWAESNODE_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEvents.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/InspectorWidget.h"

namespace Bang
{
class IEventsDestroy;
class IEventsValueChanged;
class UIInputText;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class AESNode;
class UIInputFileWithPreview;

class GIWAESNode : public InspectorWidget
{
public:
    GIWAESNode();
    virtual ~GIWAESNode() override;

    // InspectorWidget
    virtual void InitInnerWidgets() override;

    void SetAESNode(AESNode *node);

private:
    AESNode *p_aesNode = nullptr;
    UIInputText *p_nameInput = nullptr;
    UIInputFileWithPreview *p_nodeAnimationInput = nullptr;

    AESNode *GetAESNode() const;

    // InspectorWidget
    virtual void UpdateFromReference() override;

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;

    // IEventsDestroy
    virtual void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;
};
}

#endif  // GIWAESNODE_H
