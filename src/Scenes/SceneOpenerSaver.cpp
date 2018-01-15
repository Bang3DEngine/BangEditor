#include "BangEditor/SceneOpenerSaver.h"

#include "Bang/Dialog.h"
#include "Bang/Extensions.h"
#include "Bang/XMLNodeReader.h"

#include "BangEditor/MenuBar.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorScene.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

SceneOpenerSaver::SceneOpenerSaver()
{
    SceneManager::GetInstance()->
        EventEmitter<SceneManagerListener>::RegisterListener(this);
}

SceneOpenerSaver::~SceneOpenerSaver()
{
}

bool SceneOpenerSaver::OnNewScene()
{
    if (CloseScene())
    {
        EditorScene *edScene = EditorSceneManager::GetEditorScene();

        Scene *defaultScene = GameObjectFactory::CreateScene();
        edScene->SetOpenScene(defaultScene);

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
            if (CloseScene())
            {
                EditorScene *edScene = EditorSceneManager::GetEditorScene();
                Scene *scene = GameObjectFactory::CreateScene(false);
                scene->ImportXMLFromFile(openScenePath);
                edScene->SetOpenScene(scene);

                m_currentOpenScenePath = openScenePath;
                return true;
            }
        }
    }
    return false;
}

bool SceneOpenerSaver::OnSaveScene() { return OnSaveScene(false); }
bool SceneOpenerSaver::OnSaveSceneAs() { return OnSaveScene(true); }

void SceneOpenerSaver::OnSceneOpen(Scene*, const Path &sceneFilepath)
{
    m_currentOpenScenePath = sceneFilepath;
}

SceneOpenerSaver *SceneOpenerSaver::GetInstance()
{
    return MenuBar::GetInstance()->m_sceneOpenerSaver;
}

bool SceneOpenerSaver::OnSaveScene(bool saveAs)
{
    EditorScene *edScene = EditorSceneManager::GetEditorScene();
    Scene *openScene = edScene->GetOpenScene();
    if (openScene)
    {
        Path saveScenePath = m_currentOpenScenePath;
        if (saveAs || !saveScenePath.IsFile())
        {
            saveScenePath = Dialog::SaveFilePath("Save Scene As...",
                                        { Extensions::GetSceneExtension() },
                                        GetDialogStartPath(),
                                        m_currentOpenScenePath.GetName());
        }

        bool saveScene = true;
        if (saveScenePath.IsFile() && saveScenePath != m_currentOpenScenePath)
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
        }
        else if (saveSceneYNC == Dialog::Cancel) { return false; }
    }

    m_currentOpenScenePath = Path::Empty;
    EditorSceneManager::GetEditorScene()->SetOpenScene(nullptr);
    return true;
}

Path SceneOpenerSaver::GetDialogStartPath() const
{
    if (EditorPaths::GetProjectAssetsDir().Exists())
    {
        return EditorPaths::GetProjectAssetsDir();
    }
    return Paths::GetHome();
}

bool SceneOpenerSaver::IsCurrentSceneSaved() const
{
    Scene *openScene = EditorSceneManager::GetOpenScene();
    if (!openScene) { return false; }

    if (m_currentOpenScenePath.IsFile())
    {
        XMLNode savedInfo = XMLNodeReader::FromFile( m_currentOpenScenePath );
        XMLNode sceneInfo;
        openScene->ExportXML(&sceneInfo);

        return (savedInfo.ToString() == sceneInfo.ToString());
    }
    return false;
}

