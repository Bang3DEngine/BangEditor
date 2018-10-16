#ifndef GIWAESNODE_H
#define GIWAESNODE_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsValueChanged.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/InspectorWidget.h"

namespace Bang {
class IEventsDestroy;
class IEventsValueChanged;
template <class > class EventEmitter;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIInputText;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class AESNode;
FORWARD class UIInputFileWithPreview;

class GIWAESNode : public InspectorWidget,
                   public EventListener<IEventsValueChanged>
{
public:
	GIWAESNode();
	virtual ~GIWAESNode();

    // InspectorWidget
    virtual void InitInnerWidgets() override;

    void SetAESNode(AESNode *node);

private:
    AESNode *p_aesNode =  nullptr;
    UIInputText *p_nameInput = nullptr;
    UIInputFileWithPreview *p_nodeAnimationInput = nullptr;

    AESNode* GetAESNode() const;

    // InspectorWidget
    virtual void UpdateFromReference() override;

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;

    // IEventsDestroy
    virtual void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // GIWAESNODE_H

