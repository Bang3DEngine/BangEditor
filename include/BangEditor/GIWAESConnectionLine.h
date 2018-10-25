#ifndef GIWAESCONNECTIONLINE_H
#define GIWAESCONNECTIONLINE_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsDestroy.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/InspectorWidget.h"

namespace Bang
{
class AnimatorStateMachineTransition;
class AnimatorStateMachineNode;
class GameObject;
class IEventsDestroy;
class IEventsValueChanged;
class UICheckBox;
class UIInputNumber;
class UIInputText;
class UILabel;
class UIList;
class UISlider;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class AESConnectionLine;
class UIInputArray;

class GIWAESConnectionLine : public InspectorWidget
{
public:
    GIWAESConnectionLine();
    virtual ~GIWAESConnectionLine() override;

    // InspectorWidget
    virtual void InitInnerWidgets() override;

    void SetAESConnectionLine(AESConnectionLine *connLine);

private:
    AESConnectionLine *p_aesConnectionLine = nullptr;
    AnimatorStateMachineTransition *p_selectedSMTransition = nullptr;

    UIList *p_transitionsList = nullptr;
    GameObject *p_transitionsListSeparator = nullptr;

    UICheckBox *p_immediateTransitionInput = nullptr;
    UIInputNumber *p_transitionDurationInput = nullptr;
    UILabel *p_notificationLabel = nullptr;
    UIInputArray *p_transitionConditionsInput = nullptr;

    void EnableNeededWidgets();
    void SetSelectedSMTransition(AnimatorStateMachineTransition *transition);

    AnimatorStateMachineNode *GetSMNodeTo() const;
    AnimatorStateMachineNode *GetSMNodeFrom() const;
    AESConnectionLine *GetAESConnectionLine() const;
    AnimatorStateMachineTransition *GetSelectedSMTransition() const;

    // InspectorWidget
    virtual void UpdateFromReference() override;

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;

    // IEventsDestroy
    virtual void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;
};
}

#endif  // GIWAESCONNECTIONLINE_H
