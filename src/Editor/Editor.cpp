#include "BangEditor/Editor.h"

#include "Bang/AudioManager.h"
#include "Bang/IEventsSceneManager.h"

#include "BangEditor/EditorSettings.h"
#include "BangEditor/UndoRedoManager.h"
#include "BangEditor/EditorApplication.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/NotSelectableInEditor.h"
#include "BangEditor/UndoRedoGameObjectSelection.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Editor::Editor()
{
    m_editorSettings = new EditorSettings();
    GetEditorSettings()->Init();
}

Editor::~Editor()
{
    delete m_editorSettings;
}

void Editor::Init()
{
    SceneManager::GetActive()->
                  EventEmitter<IEventsSceneManager>::RegisterListener(this);
}

GameObject *Editor::GetSelectedGameObject()
{
    Editor *ed = Editor::GetInstance();
    return ed ? ed->p_selectedGameObject : nullptr;
}

void Editor::SelectGameObject(GameObject *selectedGameObject)
{
    Editor *ed = Editor::GetInstance();
    if (ed) { ed->SelectGameObject_(selectedGameObject); }
}

void Editor::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    if (GetSelectedGameObject() == object)
    {
        SelectGameObject_(nullptr);
    }
}

void Editor::SelectGameObject_(GameObject *selectedGameObject, bool registerUndo)
{
    bool isSelectable = !selectedGameObject ||
         (!selectedGameObject->GetComponent<NotSelectableInEditor>() &&
          !selectedGameObject->GetComponentInParent<NotSelectableInEditor>());

    if (selectedGameObject != GetSelectedGameObject())
    {
        if (registerUndo && isSelectable)
        {
            UndoRedoGameObjectSelection *undoRedo =
                    new UndoRedoGameObjectSelection(GetSelectedGameObject(),
                                                    selectedGameObject);
            UndoRedoManager::PushAction(undoRedo);
        }

        if (isSelectable)
        {
            p_selectedGameObject = selectedGameObject;
            if (GetSelectedGameObject())
            {
                GetSelectedGameObject()->
                        EventEmitter<IEventsDestroy>::RegisterListener(this);
            }

            // Propagate event
            EventEmitter<IEventsEditor>::
                PropagateToListeners(&EventListener<IEventsEditor>::OnGameObjectSelected,
                                     GetSelectedGameObject());
        }
    }
}

void Editor::OnPathSelected(const Path &path)
{
    Editor *ed = Editor::GetInstance(); ASSERT(ed);
    if (path.IsFile())
    {
        ed->SelectGameObject_(nullptr, false);
    }

    ed->EventEmitter<IEventsEditor>::PropagateToListeners(
                &IEventsEditor::OnExplorerPathSelected, path);
}

EditorSettings *Editor::GetEditorSettings() const
{
    return m_editorSettings;
}

void Editor::OnSceneLoaded(Scene*, const Path &)
{
    SelectGameObject(nullptr);
}

Editor *Editor::GetInstance()
{
    EditorApplication *edApp = EditorApplication::GetInstance();
    return edApp ? edApp->GetEditor() : nullptr;
}

