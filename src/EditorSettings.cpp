#include "BangEditor/EditorSettings.h"

#include "Bang/File.h"
#include "Bang/XMLNode.h"
#include "Bang/XMLNodeReader.h"

#include "BangEditor/Editor.h"
#include "BangEditor/EditorPaths.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void EditorSettings::SetLatestProjectFilepathOpen(const Path &latestProjectFilePathOpen)
{
    EditorSettings *es = EditorSettings::GetInstance();
    ASSERT(es);

    es->m_latestProjectFileOpen = latestProjectFilePathOpen;

    es->ExportToFile();
}

Path EditorSettings::GetLatestProjectFilepathOpen()
{
    EditorSettings *es = EditorSettings::GetInstance();
    ASSERT(es);
    return es->m_latestProjectFileOpen;
}

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

void EditorSettings::ExportToFile()
{
    XMLNode xmlInfo;
    xmlInfo.SetTagName("EditorSettings");
    xmlInfo.Set("LatestProjectFileOpen", m_latestProjectFileOpen);

    File::Write(EditorSettings::GetEditorSettingsPath(), xmlInfo.ToString());
}

void EditorSettings::ImportFromFile()
{
    const Path editorSettingsPath = EditorSettings::GetEditorSettingsPath();

    XMLNode settingsXML = XMLNodeReader::FromFile(editorSettingsPath);

    if (settingsXML.Contains("LatestProjectFileOpen"))
    {
        EditorSettings::SetLatestProjectFilepathOpen(
                    settingsXML.Get<Path>("LatestProjectFileOpen"));
    }
}

Path EditorSettings::GetEditorSettingsPath()
{
    return EditorPaths::GetEditorResourcesDir().Append("EditorSettings")
                                         .AppendExtension("txt");
}

EditorSettings *EditorSettings::GetInstance()
{
    Editor *ed = Editor::GetInstance();
    return ed ? ed->GetEditorSettings() : nullptr;
}

