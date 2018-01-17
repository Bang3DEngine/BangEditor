#ifndef UISCENETOOLBAR_H
#define UISCENETOOLBAR_H

#include "Bang/GameObject.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IValueChangedListener.h"

#include "BangEditor/Editor.h"
#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIButton;
FORWARD class Texture2D;
FORWARD class UICheckBox;
FORWARD class UITextRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UISceneToolbar : public GameObject,
                       public EventEmitter<IValueChangedListener>,
                       public IEditorListener,
                       public IValueChangedListener
{
    GAMEOBJECT_EDITOR(UISceneToolbar);

public:
	UISceneToolbar();
	virtual ~UISceneToolbar();

    bool IsShowDebugStatsChecked() const;

private:
    UIButton *p_playButton = nullptr;
    UIButton *p_stopButton = nullptr;
    UITextRenderer *p_stateText = nullptr;
    UICheckBox *p_showDebugStatsCheckbox = nullptr;

    void OnPlayScene();
    void OnStopScene();

    // IEditorListener;
    void OnPlayStateChanged(EditorPlayState previousPlayState,
                            EditorPlayState newPlayState) override;

    // IValueChangedListener
    void OnValueChanged(Object *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UISCENETOOLBAR_H

