#ifndef CIWAUDIOLISTENER_H
#define CIWAUDIOLISTENER_H

#include "Bang/Bang.h"

#include "BangEditor/ComponentInspectorWidget.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class CIWAudioListener : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWAudioListener);

public:
    // InspectorWidget
    virtual void Init() override;

    // ComponentInspectorWidget
    virtual void UpdateValuesFromComponent() override;

public:
    CIWAudioListener() = default;
    virtual ~CIWAudioListener() = default;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWAUDIOLISTENER_H

