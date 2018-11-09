#include "BangEditor/Editor.h"

#include "Bang/Assert.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/IEventsDestroy.h"
#include "Bang/IEventsSceneManager.h"
#include "Bang/Path.h"
#include "Bang/SceneManager.h"
#include "BangEditor/EditorApplication.h"
#include "BangEditor/IEventsEditor.h"
#include "BangEditor/NotSelectableInEditor.h"
#include "BangEditor/PlayState.h"
#include "BangEditor/ScenePlayer.h"
#include "BangEditor/UndoRedoGameObjectSelection.h"
#include "BangEditor/UndoRedoManager.h"

using namespace Bang;
using namespace BangEditor;

Editor::Editor()
{
}

Editor::~Editor()
{
}

void Editor::Init()
{
    SceneManager::GetActive()
        ->EventEmitter<IEventsSceneManager>::RegisterListener(this);
}

GameObject *Editor::GetSelectedGameObject()
{
    Editor *ed = Editor::GetInstance();
    return ed ? ed->p_selectedGameObject : nullptr;
}

void Editor::SelectGameObject(GameObject *selectedGameObject, bool registerUndo)
{
    Editor *ed = Editor::GetInstance();
    ASSERT(ed);
    ed->SelectGameObject_(selectedGameObject, registerUndo);
}

void Editor::SelectGameObject_(GameObject *selectedGameObject,
                               bool registerUndo)
{
    bool isSelectable =
        !selectedGameObject ||
        (!selectedGameObject->GetComponent<NotSelectableInEditor>() &&
         !selectedGameObject->GetComponentInAncestors<NotSelectableInEditor>());

    if (selectedGameObject != GetSelectedGameObject())
    {
        if (false && registerUndo && isSelectable)
        {
            UndoRedoManager::PushAction(new UndoRedoGameObjectSelection(
                GetSelectedGameObject(), selectedGameObject));
        }

        if (isSelectable)
        {
            p_selectedGameObject = selectedGameObject;
            if (GetSelectedGameObject())
            {
                GetSelectedGameObject()
                    ->EventEmitter<IEventsDestroy>::RegisterListener(this);
            }

            // Propagate event
            EventEmitter<IEventsEditor>::PropagateToListeners(
                &EventListener<IEventsEditor>::OnGameObjectSelected,
                GetSelectedGameObject());
        }
    }
}

void Editor::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    if (GetSelectedGameObject() == object)
    {
        Editor::SelectGameObject(nullptr, false);
    }
}

void Editor::OnPathSelected(const Path &path)
{
    if (path.IsFile())
    {
        Editor::SelectGameObject(nullptr, false);
    }

    Editor *ed = Editor::GetInstance();
    ASSERT(ed);
    ed->EventEmitter<IEventsEditor>::PropagateToListeners(
        &IEventsEditor::OnExplorerPathSelected, path);
}

bool Editor::IsEditingScene()
{
    return ScenePlayer::GetPlayState() == PlayState::EDITING;
}
void Editor::OnSceneLoaded(Scene *, const Path &)
{
    Editor::SelectGameObject(nullptr, false);
}

Editor *Editor::GetInstance()
{
    EditorApplication *edApp = EditorApplication::GetInstance();
    return edApp ? edApp->GetEditor() : nullptr;
}
