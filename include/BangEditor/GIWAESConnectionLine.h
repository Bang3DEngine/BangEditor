#ifndef GIWAESCONNECTIONLINE_H
#define GIWAESCONNECTIONLINE_H

#include "Bang/Bang.h"
#include "Bang/IEventsDestroy.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/InspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIInputText;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputArray;
FORWARD class AESConnectionLine;

class GIWAESConnectionLine : public InspectorWidget,
                             public EventListener<IEventsDestroy>,
                             public EventListener<IEventsValueChanged>
{
public:
	GIWAESConnectionLine();
	virtual ~GIWAESConnectionLine();

    // InspectorWidget
    virtual void Init() override;

    void SetAESConnectionLine(AESConnectionLine *connLine);

private:
    UIInputArray *p_transitionConditionsInput = nullptr;
    AESConnectionLine *p_aesConnectionLine =  nullptr;

    AESConnectionLine* GetAESConnectionLine() const;

    // InspectorWidget
    virtual void UpdateFromReference() override;

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;

    // IEventsDestroy
    virtual void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;

};

NAMESPACE_BANG_EDITOR_END

#endif // GIWAESCONNECTIONLINE_H

