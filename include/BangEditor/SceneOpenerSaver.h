#ifndef SCENEOPENERSAVER_H
#define SCENEOPENERSAVER_H

#include "Bang/Path.h"
#include "Bang/Dialog.h"

#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class SceneOpenerSaver : public ISceneManagerListener
{
public:
	SceneOpenerSaver();
	virtual ~SceneOpenerSaver();

    bool OnNewScene();
    bool OnOpenScene();
    bool OnSaveScene();
    bool OnSaveSceneAs();
    bool CloseScene();
    bool IsCurrentSceneSaved() const;

    bool OpenSceneInEditor(const Path &scenePath);

    // SceneManagerListener
    void OnSceneLoaded(Scene *scene, const Path &sceneFilepath) override;

    static SceneOpenerSaver* GetInstance();

private:
    Path m_currentOpenScenePath = Path::Empty;

    bool OnSaveScene(bool saveAs);
    Dialog::YesNoCancel Overwrite(const Path &path);

    Path GetDialogStartPath() const;
};

NAMESPACE_BANG_EDITOR_END

#endif // SCENEOPENERSAVER_H

