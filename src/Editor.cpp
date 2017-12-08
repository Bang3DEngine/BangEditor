#include "BangEditor/Editor.h"

#include "BangEditor/EditorScene.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void Editor::SelectGameObject(GameObject *selectedGameObject)
{
    Editor *ed = Editor::GetInstance();
    if (ed && ed->p_selectedGameObject != selectedGameObject)
    {
        ed->p_selectedGameObject = selectedGameObject;
        ed->_EmitGameObjectSelected(selectedGameObject);
    }
}

void Editor::_EmitGameObjectSelected(GameObject *selectedGameObject)
{
    EventEmitter<IEditorSelectionListener>::
        PropagateToListeners(&IEditorSelectionListener::OnGameObjectSelected,
                             selectedGameObject);
}

Editor *Editor::GetInstance()
{
    EditorScene *edScene = EditorScene::GetInstance();
    return edScene ? edScene->GetEditor() : nullptr;
}

