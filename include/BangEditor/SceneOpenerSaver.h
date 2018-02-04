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

    // ISceneManagerListener
    void OnSceneLoaded(Scene *scene, const Path &sceneFilepath) override;

    const Path& GetOpenScenePath() const;
    const Path& GetLoadedScenePath() const;

    static SceneOpenerSaver* GetInstance();

private:
    Path m_currentLoadedScenePath = Path::Empty;
    Path m_currentOpenScenePath = Path::Empty;

    bool OnSaveScene(bool saveAs);
    Dialog::YesNoCancel Overwrite(const Path &path);

    Path GetDialogStartPath() const;
};

NAMESPACE_BANG_EDITOR_END

#endif // SCENEOPENERSAVER_H

