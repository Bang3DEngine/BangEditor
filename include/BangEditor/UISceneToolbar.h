#ifndef UISCENETOOLBAR_H
#define UISCENETOOLBAR_H

#include "Bang/GameObject.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IValueChangedListener.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/ScenePlayer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIButton;
FORWARD class Texture2D;
FORWARD class UICheckBox;
FORWARD class UIComboBox;
FORWARD class UITextRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UISceneToolbar : public GameObject,
                       public EventEmitter<IValueChangedListener>,
                       public IScenePlayerListener,
                       public IValueChangedListener
{
    GAMEOBJECT_EDITOR(UISceneToolbar);

public:
	UISceneToolbar();
	virtual ~UISceneToolbar();

    void Update() override;

    bool IsShowDebugStatsChecked() const;
    UIComboBox* GetRenderModeComboBox() const;

private:
    UIButton *p_playButton  = nullptr;
    UIButton *p_pauseButton = nullptr;
    UIButton *p_stepButton  = nullptr;
    UIButton *p_stopButton  = nullptr;

    UIButton *p_translateButton      = nullptr;
    UIButton *p_rotateButton         = nullptr;
    UIButton *p_scaleButton          = nullptr;
    UIButton *p_rectTransformButton  = nullptr;

    UIComboBox *p_renderModeInput = nullptr;
    UICheckBox *p_showDebugStatsCheckbox = nullptr;

    void OnPlayScene();
    void OnPauseScene();
    void OnStopScene();

    void SetRenderMode();

    // IScenePlayerListener
    void OnPlayStateChanged(PlayState previousPlayState,
                            PlayState newPlayState) override;

    // IValueChangedListener
    void OnValueChanged(Object *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UISCENETOOLBAR_H

