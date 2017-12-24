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

void Editor::SelectGameObject(GameObject *selectedGameObject)
{
    Editor *ed = Editor::GetInstance();
    if (ed) { ed->_SelectGameObject(selectedGameObject); }
}

void Editor::OnDestroyed(Object *object)
{
    if (p_selectedGameObject == object)
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

    if (p_selectedGameObject != selectedGameObject && isSelectable)
    {
        p_selectedGameObject = selectedGameObject;
        if (p_selectedGameObject)
        {
            p_selectedGameObject->
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
        if (p_selectedGameObject)
        {
            ASSERT(!p_currentTransformGizmo);
            TransformGizmo *tg = GameObject::Create<TransformGizmo>();
            tg->SetReferencedGameObject(p_selectedGameObject);
            p_currentTransformGizmo = tg;
            p_currentTransformGizmo->
                    EventEmitter<IDestroyListener>::RegisterListener(this);

            Scene *openScene = EditorSceneManager::GetOpenScene();
            p_currentTransformGizmo->SetParent(openScene);
        }
    }
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

