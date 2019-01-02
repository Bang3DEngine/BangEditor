#include "BangEditor/SIWBuildSettings.h"

#include "Bang/Extensions.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsValueChanged.h"
#include "BangEditor/EditorProject.h"
#include "BangEditor/EditorProjectManager.h"
#include "BangEditor/UIInputFile.h"

using namespace Bang;
using namespace BangEditor;

SIWBuildSettings::SIWBuildSettings()
{
}

SIWBuildSettings::~SIWBuildSettings()
{
}

void SIWBuildSettings::InitInnerWidgets()
{
    SettingsInspectorWidget::InitInnerWidgets();

    SetTitle("Build Settings");
    SetName("SIWBuildSettings");

    p_initialSceneInput = new UIInputFile();
    p_initialSceneInput->SetExtensions({Extensions::GetSceneExtension()});
    p_initialSceneInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    AddWidget("Initial scene", p_initialSceneInput);
    SetLabelsWidth(100);

    Update();
}

void SIWBuildSettings::UpdateFromReference()
{
    SettingsInspectorWidget::UpdateFromReference();

    EditorProject *editorProject =
        EditorProjectManager::GetInstance()->GetCurrentProject();
    if (editorProject)
    {
        Path initialScenePath = editorProject->GetInitialScenePath();
        p_initialSceneInput->SetPath(initialScenePath);
    }
}

void SIWBuildSettings::OnValueChanged(EventEmitter<IEventsValueChanged> *ee)
{
    SettingsInspectorWidget::OnValueChanged(ee);

    EditorProject *editorProject =
        EditorProjectManager::GetInstance()->GetCurrentProject();
    if (editorProject)
    {
        editorProject->SetInitialScenePath(p_initialSceneInput->GetPath());
        editorProject->ExportToProjectFile();
    }
}
