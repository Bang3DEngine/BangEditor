#include "BangEditor/CIWAudioListener.h"

using namespace Bang;
using namespace BangEditor;

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
