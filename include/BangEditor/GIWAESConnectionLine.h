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
#include "Bang/IEventsValueChanged.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/InspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class AnimatorStateMachineConnection;
FORWARD   class AnimatorStateMachineNode;
FORWARD   class GameObject;
FORWARD   class IEventsDestroy;
FORWARD   class IEventsValueChanged;
FORWARD   class UICheckBox;
FORWARD   class UIInputNumber;
FORWARD   class UIInputText;
FORWARD   class UILabel;
FORWARD   class UIList;
FORWARD   class UISlider;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class AESConnectionLine;
FORWARD class UIInputArray;

class GIWAESConnectionLine : public InspectorWidget,
                             public EventListener<IEventsValueChanged>
{
public:
	GIWAESConnectionLine();
	virtual ~GIWAESConnectionLine() override;

    // InspectorWidget
    virtual void InitInnerWidgets() override;

    void SetAESConnectionLine(AESConnectionLine *connLine);

private:
    AESConnectionLine *p_aesConnectionLine = nullptr;
    AnimatorStateMachineConnection *p_selectedSMConnection = nullptr;

    UIList *p_transitionsList = nullptr;
    GameObject *p_transitionsListSeparator = nullptr;

    UICheckBox *p_immediateTransitionInput = nullptr;
    UIInputNumber *p_transitionDurationInput = nullptr;
    UILabel *p_notificationLabel = nullptr;
    UIInputArray *p_transitionConditionsInput = nullptr;

    void EnableNeededWidgets();
    void SetSelectedSMConnection(AnimatorStateMachineConnection *connection);

    AnimatorStateMachineNode *GetSMNodeTo() const;
    AnimatorStateMachineNode *GetSMNodeFrom() const;
    AESConnectionLine* GetAESConnectionLine() const;
    AnimatorStateMachineConnection *GetSelectedSMConnection() const;

    // InspectorWidget
    virtual void UpdateFromReference() override;

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;

    // IEventsDestroy
    virtual void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;

};

NAMESPACE_BANG_EDITOR_END

#endif // GIWAESCONNECTIONLINE_H

