#include "BangEditor/Editor.h"

#include "BangEditor/EditorScene.h"
#include "BangEditor/TransformGizmo.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void Editor::SelectGameObject(GameObject *selectedGameObject)
{
    Editor *ed = Editor::GetInstance();
    if (ed) { ed->_SelectGameObject(selectedGameObject); }
}

void Editor::_SelectGameObject(GameObject *selectedGameObject)
{
    bool isValid = selectedGameObject &&
            (!p_currentTransformGizmo ||
             !selectedGameObject->IsChildOf(p_currentTransformGizmo));

    if (p_selectedGameObject != selectedGameObject && isValid)
    {
        p_selectedGameObject = selectedGameObject;

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
            TransformGizmo *tg = GameObject::Create<TransformGizmo>();
            tg->SetReferencedGameObject(p_selectedGameObject);
            p_currentTransformGizmo = tg;

            Scene *openScene = EditorSceneManager::GetOpenScene();
            p_currentTransformGizmo->SetParent(openScene);
        }
    }
}

Editor *Editor::GetInstance()
{
    EditorScene *edScene = EditorSceneManager::GetEditorScene();
    return edScene ? edScene->GetEditor() : nullptr;
}

