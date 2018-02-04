#include "BangEditor/SceneOpenerSaver.h"

#include "Bang/Debug.h"
#include "Bang/Dialog.h"
#include "Bang/Extensions.h"
#include "Bang/XMLNodeReader.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/Editor.h"
#include "BangEditor/MenuBar.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorScene.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

SceneOpenerSaver::SceneOpenerSaver()
{
    SceneManager::GetActive()->
        EventEmitter<ISceneManagerListener>::RegisterListener(this);
}

SceneOpenerSaver::~SceneOpenerSaver()
{
}

bool SceneOpenerSaver::OnNewScene()
{
    if (CloseScene())
    {
        Scene *defaultScene = GameObjectFactory::CreateScene();
        SceneManager::LoadSceneInstantly(defaultScene);

        GameObjectFactory::CreateDefaultSceneInto(defaultScene);
        defaultScene->SetFirstFoundCamera();
        return true;
    }
    return false;
}

bool SceneOpenerSaver::OnOpenScene()
{
    Path openScenePath = Dialog::OpenFilePath("Open Scene...",
                                             { Extensions::GetSceneExtension() },
                                              GetDialogStartPath());
    if (!openScenePath.IsEmpty())
    {
        if (openScenePath.IsFile())
        {
            m_currentOpenScenePath = openScenePath;
            return OpenSceneInEditor(openScenePath);
        }
    }
    return false;
}

bool SceneOpenerSaver::OnSaveScene() { return OnSaveScene(false); }
bool SceneOpenerSaver::OnSaveSceneAs() { return OnSaveScene(true); }

void SceneOpenerSaver::OnSceneLoaded(Scene*, const Path &sceneFilepath)
{
    m_currentLoadedScenePath = sceneFilepath;
}

const Path &SceneOpenerSaver::GetLoadedScenePath() const
{
    return m_currentLoadedScenePath;
}

const Path &SceneOpenerSaver::GetOpenScenePath() const
{
    return m_currentOpenScenePath;
}

SceneOpenerSaver *SceneOpenerSaver::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetSceneOpenerSaver();
}

bool SceneOpenerSaver::OnSaveScene(bool saveAs)
{
    EditorScene *edScene = EditorSceneManager::GetEditorScene();
    Scene *openScene = edScene->GetOpenScene();
    if (openScene && Editor::IsEditingScene())
    {
        Path saveScenePath = GetOpenScenePath();
        if (saveAs || !saveScenePath.IsFile())
        {
            saveScenePath = Dialog::SaveFilePath("Save Scene As...",
                            { Extensions::GetSceneExtension() },
                             GetDialogStartPath(),
                             GetOpenScenePath().GetName());
        }

        bool saveScene = true;
        if (saveScenePath.IsFile() && saveScenePath != GetOpenScenePath())
        {
            Dialog::YesNoCancel yesNoCancel = Overwrite(saveScenePath);
            saveScene = (yesNoCancel == Dialog::Yes);
        }

        if (saveScene)
        {
            m_currentOpenScenePath = saveScenePath;
            openScene->ExportXMLToFile( Path(saveScenePath) );
        }

        return true;
    }
    return false;
}

Dialog::YesNoCancel SceneOpenerSaver::Overwrite(const Path &path)
{
    return Dialog::GetYesNoCancel("Overwrite?",
                         "Are you sure you want to overwrite the file '" +
                         path.GetNameExt() + "'?");
}

bool SceneOpenerSaver::CloseScene()
{
    if (!IsCurrentSceneSaved())
    {
        Dialog::YesNoCancel saveSceneYNC =
                Dialog::GetYesNoCancel("Save current scene",
                                       "Current scene is not saved."
                                       " Do you want to save it ?");

        if (saveSceneYNC == Dialog::Yes)
        {
            if (!OnSaveScene()) { return false; }
            else
            {
                Scene *previousScene = SceneManager::GetActiveScene();
                if (previousScene) { GameObject::Destroy(previousScene); }
            }
        }
        else if (saveSceneYNC == Dialog::Cancel) { return false; }
    }

    m_currentOpenScenePath = Path::Empty;
    SceneManager::LoadScene(nullptr);
    return true;
}

Path SceneOpenerSaver::GetDialogStartPath() const
{
    if (Paths::GetProjectAssetsDir().Exists())
    {
        return Paths::GetProjectAssetsDir();
    }
    return Paths::GetHome();
}

bool SceneOpenerSaver::IsCurrentSceneSaved() const
{
    Scene *openScene = EditorSceneManager::GetOpenScene();
    if (!openScene) { return true; }

    if (GetOpenScenePath().IsFile())
    {
        XMLNode savedInfo = XMLNodeReader::FromFile( GetOpenScenePath() );
        XMLNode sceneInfo;
        openScene->ExportXML(&sceneInfo);

        return (savedInfo.ToString() == sceneInfo.ToString());
    }
    return false;
}

bool SceneOpenerSaver::OpenSceneInEditor(const Path &scenePath)
{
    if (CloseScene())
    {
        SceneManager::LoadScene(scenePath);
        m_currentOpenScenePath = scenePath;
        return true;
    }
    return false;
}

