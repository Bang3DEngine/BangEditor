#ifndef SCENETAB_H
#define SCENETAB_H

#include "Bang/GameObject.h"

#include "BangEditor/Editor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UITextRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UISceneContainer;

class SceneTab : public GameObject,
                 public IEditorSelectionListener
{
    GAMEOBJECT_EDITOR(SceneTab);

public:
	SceneTab();
	virtual ~SceneTab();

    void OnPlayScene();
    void OnStopScene();

    void SetSceneImageTexture(Texture2D *sceneTexture);
    Rect GetSceneImageRectNDC() const;

    // IEditorSelectionListener;
    void OnPlayStateChanged(EditorPlayState previousPlayState,
                            EditorPlayState newPlayState) override;

private:
    Scene *p_editingScene = nullptr;
    Scene *p_playingScene = nullptr;

    UIButton *p_playButton = nullptr;
    UIButton *p_stopButton = nullptr;
    UITextRenderer *p_stateText = nullptr;
    UISceneContainer *p_sceneContainer = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // SCENETAB_H

