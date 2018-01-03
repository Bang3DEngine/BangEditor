#include "BangEditor/Editor.h"

#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSettings.h"
#include "BangEditor/TransformGizmo.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/NotSelectableInEditor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Editor::Editor()
{
}

Editor::~Editor()
{
    delete m_editorSettings;
}

void Editor::Init()
{
    m_editorSettings = new EditorSettings();
    GetEditorSettings()->Init();
}

GameObject *Editor::GetSelectedGameObject()
{
    Editor *ed = Editor::GetInstance();
    return ed ? ed->p_selectedGameObject : nullptr;
}

void Editor::SelectGameObject(GameObject *selectedGameObject)
{
    Editor *ed = Editor::GetInstance();
    if (ed) { ed->_SelectGameObject(selectedGameObject); }
}

void Editor::OnDestroyed(Object *object)
{
    if (GetSelectedGameObject() == object)
    {
        _SelectGameObject(nullptr);
    }

    if (p_currentTransformGizmo == object)
    {
        p_currentTransformGizmo = nullptr;
    }
}

void Editor::_SelectGameObject(GameObject *selectedGameObject)
{
    bool isSelectable = !selectedGameObject ||
         (!selectedGameObject->GetComponent<NotSelectableInEditor>() &&
          !selectedGameObject->GetComponentInParent<NotSelectableInEditor>());

    if (selectedGameObject != GetSelectedGameObject() && isSelectable)
    {
        p_selectedGameObject = selectedGameObject;
        if (GetSelectedGameObject())
        {
            GetSelectedGameObject()->
                    EventEmitter<IDestroyListener>::RegisterListener(this);
        }

        // Propagate event
        EventEmitter<IEditorSelectionListener>::
            PropagateToListeners(&IEditorSelectionListener::OnGameObjectSelected,
                                 selectedGameObject);

        // Destroy previous transform gizmo
        if (p_currentTransformGizmo)
        {
            GameObject::Destroy(p_currentTransformGizmo);
            p_currentTransformGizmo = nullptr;
        }

        // Create new transform gizmo
        if (GetSelectedGameObject())
        {
            ASSERT(!p_currentTransformGizmo);
            TransformGizmo *tg = GameObject::Create<TransformGizmo>();
            tg->SetReferencedGameObject(GetSelectedGameObject());
            p_currentTransformGizmo = tg;
            p_currentTransformGizmo->
                    EventEmitter<IDestroyListener>::RegisterListener(this);

            Scene *openScene = EditorSceneManager::GetOpenScene();
            p_currentTransformGizmo->SetParent(openScene);
        }
    }
}

void Editor::OnPathSelected(const Path &path)
{
    Editor *ed = Editor::GetInstance();
    ASSERT(ed);

    ed->EventEmitter<IEditorSelectionListener>::PropagateToListeners(
            &IEditorSelectionListener::OnExplorerPathSelected, path);
}

EditorSettings *Editor::GetEditorSettings() const
{
    return m_editorSettings;
}

Editor *Editor::GetInstance()
{
    EditorScene *edScene = EditorSceneManager::GetEditorScene();
    return edScene ? edScene->GetEditor() : nullptr;
}

