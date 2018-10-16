#ifndef CIWAUDIOLISTENER_H
#define CIWAUDIOLISTENER_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class CIWAudioListener : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWAudioListener);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

public:
    CIWAudioListener() = default;
    virtual ~CIWAudioListener() override = default;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWAUDIOLISTENER_H

