#include "BangEditor/EditorSettings.h"

#include "Bang/Array.tcc"
#include "Bang/Assert.h"
#include "Bang/Debug.h"
#include "Bang/File.h"
#include "Bang/MetaNode.h"
#include "Bang/MetaNode.tcc"
#include "BangEditor/Editor.h"
#include "BangEditor/EditorPaths.h"

using namespace Bang;
using namespace BangEditor;

EditorSettings::EditorSettings()
{
}

EditorSettings::~EditorSettings()
{
}

void EditorSettings::Init()
{
    ImportFromFile();
}

void EditorSettings::AddRecentProjectFilepathOpen(
    const Path &recentProjectFilePathOpen)
{
    EditorSettings *es = EditorSettings::GetInstance();
    ASSERT(es);

    if (!es->m_recentProjectFilesOpen.Contains(recentProjectFilePathOpen))
    {
        es->m_recentProjectFilesOpen.PushBack(recentProjectFilePathOpen);
        es->ExportToFile();
    }
}

const Array<Path> &EditorSettings::GetRecentProjectFilepathsOpen()
{
    EditorSettings *es = EditorSettings::GetInstance();
    ASSERT(es);

    return es->m_recentProjectFilesOpen;
}

void EditorSettings::ExportToFile()
{
    MetaNode metaNode;
    metaNode.SetName("EditorSettings");
    metaNode.SetArray("RecentProjectFilesOpen", m_recentProjectFilesOpen);

    File::Write(EditorSettings::GetEditorSettingsPath(), metaNode.ToString());
}

void EditorSettings::ImportFromFile()
{
    const Path editorSettingsPath = EditorSettings::GetEditorSettingsPath();

    MetaNode settingsMeta;
    settingsMeta.Import(editorSettingsPath);
    m_recentProjectFilesOpen =
        settingsMeta.GetArray<Path>("RecentProjectFilesOpen");

    if (settingsMeta.Contains("CompilerPath"))
    {
        Paths::SetCompilerPath(settingsMeta.Get<Path>("CompilerPath"));
    }

    ExportToFile();
}

Path EditorSettings::GetEditorSettingsPath()
{
    return EditorPaths::GetEditorAssetsDir()
        .Append("EditorSettings")
        .AppendExtension("txt");
}

EditorSettings *EditorSettings::GetInstance()
{
    return SCAST<EditorSettings *>(Settings::GetInstance());
}
