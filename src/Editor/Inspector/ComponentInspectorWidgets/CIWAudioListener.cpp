#include "BangEditor/CIWAudioListener.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void CIWAudioListener::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();
    SetName("CIWAudioListener");
    SetTitle("AudioListener");
}

void CIWAudioListener::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();
}
