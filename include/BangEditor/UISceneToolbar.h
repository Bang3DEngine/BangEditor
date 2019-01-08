#ifndef UISCENETOOLBAR_H
#define UISCENETOOLBAR_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/TransformGizmo.h"

namespace Bang
{
class IEventsValueChanged;
class Texture2D;
class UIButton;
class UICheckBox;
class UIComboBox;
class UITextRenderer;
class UIToolButton;
}

using namespace Bang;
namespace BangEditor
{
class UISceneToolbar : public GameObject,
                       public EventEmitter<IEventsValueChanged>,
                       public EventListener<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(UISceneToolbar);

public:
    UISceneToolbar();
    virtual ~UISceneToolbar() override;

    void Update() override;
    void SetIsEditToolbar(bool isEditToolbar);
    void SetTransformGizmoMode(TransformGizmoMode transformMode);

    TransformGizmoMode GetTransformGizmoMode() const;
    TransformGizmoCoordSpace GetTransformGizmoCoordSpace() const;

    bool IsShowDebugStatsChecked() const;
    void DisableTransformAndCameraControls();
    UIComboBox *GetRenderModeComboBox() const;

    static UISceneToolbar *GetActive();

private:
    UIToolButton *p_translateButton = nullptr;
    UIToolButton *p_rotateButton = nullptr;
    UIToolButton *p_scaleButton = nullptr;
    UIToolButton *p_rectTransformButton = nullptr;
    UIToolButton *p_globalLocalButton = nullptr;
    UIToolButton *p_seePostProcessButton = nullptr;

    GameObject *p_transformWorldLocalSeparator = nullptr;
    GameObject *p_transformModeCamSeparator = nullptr;
    GameObject *p_postProcessSeparator = nullptr;
    GameObject *p_transformModeCamSpacer = nullptr;
    UIToolButton *p_resetCamViewButton = nullptr;

    UIToolButton *p_playButton = nullptr;
    UIToolButton *p_pauseButton = nullptr;
    UIToolButton *p_stepButton = nullptr;
    UIToolButton *p_stopButton = nullptr;

    bool m_isEditToolbar = false;
    TransformGizmoMode m_transformGizmoMode = TransformGizmoMode::TRANSLATE;

    UIComboBox *p_renderModeInput = nullptr;
    UICheckBox *p_showDebugStatsCheckbox = nullptr;

    void ResetCameraView();
    void UpdateToolButtons();

    void SetRenderMode();

    // IEventsValueChanged
    void OnValueChanged(EventEmitter<IEventsValueChanged> *object) override;
};
}

#endif  // UISCENETOOLBAR_H
