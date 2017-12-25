#ifndef SCENEOPENERSAVER_H
#define SCENEOPENERSAVER_H

#include "Bang/Path.h"
#include "Bang/Dialog.h"

#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class SceneOpenerSaver : public SceneManagerListener
{
public:
	SceneOpenerSaver();
	virtual ~SceneOpenerSaver();

    bool OnNewScene();
    bool OnOpenScene();
    bool OnSaveScene();
    bool OnSaveSceneAs();

    // SceneManagerListener
    void OnSceneOpen(Scene *scene, const Path &sceneFilepath) override;

    static SceneOpenerSaver* GetInstance();

private:
    Path m_currentOpenScenePath = Path::Empty;

    bool OnSaveScene(bool saveAs);
    Dialog::YesNoCancel Overwrite(const Path &path);

    bool CloseScene();
    Path GetDialogStartPath() const;
    bool IsCurrentSceneSaved() const;
};

NAMESPACE_BANG_EDITOR_END

#endif // SCENEOPENERSAVER_H

