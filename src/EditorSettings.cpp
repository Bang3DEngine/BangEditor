#include "BangEditor/EditorSettings.h"

#include "BangEditor/Editor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorSettings::EditorSettings()
{
}

EditorSettings::~EditorSettings()
{
}

EditorSettings *EditorSettings::GetInstance()
{
    Editor::GetInstance()->GetEditorSettings();
}

