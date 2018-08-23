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
#include "BangEditor/ScenePlayer.h"
#include "BangEditor/UndoRedoManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

SceneOpenerSaver::SceneOpenerSaver()
{
    SceneManager::GetActive()->
        EventEmitter<IEventsSceneManager>::RegisterListener(this);
    ScenePlayer::GetInstance()->
        EventEmitter<IEventsScenePlayer>::RegisterListener(this);
}

SceneOpenerSaver::~SceneOpenerSaver()
{
}

bool SceneOpenerSaver::OnNewScene()
{
    if (!Editor::IsEditingScene())
    {
        return false;
    }

    if (CloseScene())
    {
        Scene *defaultScene = GameObjectFactory::CreateScene();
        SceneManager::LoadSceneInstantly(defaultScene, false);

        GameObjectFactory::CreateDefaultSceneInto(defaultScene);
        defaultScene->SetFirstFoundCamera();
        return true;
    }
    return false;
}

bool SceneOpenerSaver::OnOpenScene()
{
    if (!Editor::IsEditingScene())
    {
        return false;
    }

    Path openScenePath = Dialog::OpenFilePath("Open Scene...",
                                             { Extensions::GetSceneExtension() },
                                              GetDialogStartPath());
    if (!openScenePath.IsEmpty())
    {
        if (openScenePath.IsFile())
        {
            return OpenSceneInEditor(openScenePath);
        }
    }
    return false;
}

void SceneOpenerSaver::OpenDefaultScene() const
{
    Scene *scene = GameObjectFactory::CreateScene(true);
    GameObjectFactory::CreateDefaultSceneInto(scene);
    
    SceneManager::LoadScene(scene);
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
    EditorScene *edScene = EditorSceneManager::GetEditorScene();
    return edScene ? edScene->GetSceneOpenerSaver() : nullptr;
}

bool SceneOpenerSaver::OnSaveScene(bool saveAs)
{
    if (!Editor::IsEditingScene()) { return false; }

    EditorScene *edScene = EditorSceneManager::GetEditorScene();
    Scene *openScene = edScene->GetOpenScene();
    if (openScene && Editor::IsEditingScene())
    {
        Path saveScenePath = GetOpenScenePath();
        if (saveAs || !saveScenePath.IsFile())
        {
            String hintName = GetOpenScenePath().GetName();
            if (hintName.IsEmpty()) { hintName = "Scene"; }
            saveScenePath = Dialog::SaveFilePath("Save Scene As...",
                            { Extensions::GetSceneExtension() },
                             GetDialogStartPath(),
                             hintName);
        }

        bool saveScene = (saveScenePath != Path::Empty);
        if (saveScene && saveScenePath.IsFile() &&
            saveScenePath != GetOpenScenePath())
        {
            Dialog::YesNoCancel yesNoCancel = Overwrite(saveScenePath);
            saveScene = (yesNoCancel == Dialog::YES);
        }

        if (saveScene)
        {
            m_currentOpenScenePath = saveScenePath;
            m_currentLoadedScenePath = saveScenePath;
            openScene->ExportXMLToFile( Path( GetOpenScenePath() ) );
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
    Scene *previousScene = EditorSceneManager::GetOpenScene();
    if (previousScene &&
        !IsCurrentSceneSaved() &&
        ScenePlayer::GetPlayState() ==  PlayState::EDITING)
    {
        Dialog::YesNoCancel saveSceneYNC =
                Dialog::GetYesNoCancel("Save current scene",
                                       "Current scene is not saved."
                                       " Do you want to save it ?");

        if (saveSceneYNC == Dialog::YES)
        {
            if (!OnSaveScene())
            {
                return false;
            }
        }
        else if (saveSceneYNC == Dialog::CANCEL)
        {
            return false;
        }
    }

    if (previousScene)
    {
        GameObject::Destroy(previousScene);
    }

    m_currentOpenScenePath   = Path::Empty;
    m_currentLoadedScenePath = Path::Empty;
    SceneManager::LoadScene(nullptr, false);
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

void SceneOpenerSaver::OnPlayStateChanged(PlayState, PlayState newPlayState)
{
    switch (newPlayState)
    {
        case PlayState::JUST_BEFORE_PLAYING:
            m_previousLoadedScenePath = m_currentLoadedScenePath;
            m_previousOpenScenePath = m_currentOpenScenePath;
        break;

        case PlayState::PLAYING:
        case PlayState::EDITING:
            m_currentLoadedScenePath = m_previousLoadedScenePath;
            m_currentOpenScenePath = m_previousOpenScenePath;
        break;

        default: break;
    }
}

bool SceneOpenerSaver::IsCurrentSceneSaved() const
{
    Scene *openScene = EditorSceneManager::GetOpenScene();
    if (!openScene)
    {
        return false;
    }

    if (Time::GetNow_Seconds() - m_lastTimeCheckSaved >= 2.0f)
    {
        if (GetOpenScenePath().IsFile())
        {
            XMLNode savedInfo = XMLNodeReader::FromFile( GetOpenScenePath() );
            XMLNode sceneInfo;
            openScene->ExportXML(&sceneInfo);

            m_lastTimeCheckSaved = Time::GetNow_Seconds();
            m_isCurrentSceneSaved = (savedInfo.ToString() == sceneInfo.ToString());
        }
    }
    return m_isCurrentSceneSaved;
}

bool SceneOpenerSaver::OpenSceneInEditor(const Path &scenePath)
{
    if (!Editor::IsEditingScene())
    {
        return false;
    }

    if (CloseScene())
    {
        UndoRedoManager::Clear();
        SceneManager::LoadScene(scenePath, false);
        m_currentOpenScenePath = scenePath;
        m_isCurrentSceneSaved = true;
        return true;
    }
    return false;
}

