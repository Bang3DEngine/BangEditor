#ifndef CIWAUDIOLISTENER_H
#define CIWAUDIOLISTENER_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

using namespace Bang;
namespace BangEditor
{
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
}

#endif  // CIWAUDIOLISTENER_H
