#ifndef UISCENETOOLBAR_H
#define UISCENETOOLBAR_H

#include "Bang/GameObject.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/TransformGizmo.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIButton;
FORWARD class Texture2D;
FORWARD class UICheckBox;
FORWARD class UIComboBox;
FORWARD class UIToolButton;
FORWARD class UITextRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UISceneToolbar : public GameObject,
                       public EventEmitter<IEventsValueChanged>,
                       public EventListener<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR(UISceneToolbar);

public:
	UISceneToolbar();
	virtual ~UISceneToolbar();

    void Update() override;
    void SetTransformGizmoMode(TransformGizmoMode transformMode);

    TransformGizmoMode GetTransformGizmoMode() const;

    bool IsShowDebugStatsChecked() const;
    void DisableTransformAndCameraControls();
    UIComboBox* GetRenderModeComboBox() const;

    static UISceneToolbar *GetActive();

private:
    UIToolButton *p_translateButton      = nullptr;
    UIToolButton *p_rotateButton         = nullptr;
    UIToolButton *p_scaleButton          = nullptr;
    UIToolButton *p_rectTransformButton  = nullptr;
    GameObject *p_transformCamSeparator = nullptr;
    GameObject *p_transformCamSpacer = nullptr;
    UIToolButton *p_resetCamViewButton = nullptr;

    UIToolButton *p_playButton  = nullptr;
    UIToolButton *p_pauseButton = nullptr;
    UIToolButton *p_stepButton  = nullptr;
    UIToolButton *p_stopButton  = nullptr;

    TransformGizmoMode m_transformGizmoMode = TransformGizmoMode::TRANSLATE;

    UIComboBox *p_renderModeInput = nullptr;
    UICheckBox *p_showDebugStatsCheckbox = nullptr;

    void ResetCameraView();
    void UpdateToolButtons();

    void SetRenderMode();

    // IEventsValueChanged
    void OnValueChanged(EventEmitter<IEventsValueChanged> *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UISCENETOOLBAR_H

