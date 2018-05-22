#include "BangEditor/EditorDragDropManager.h"

#include "Bang/UIDragDroppable.h"

#include "BangEditor/Explorer.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorDragDropManager::EditorDragDropManager()
{
}

EditorDragDropManager::~EditorDragDropManager()
{
}

void EditorDragDropManager::OnDragStarted(UIDragDroppable *dragDroppable)
{
    if (!dragDroppable) { return; }

    if (Explorer *explorer = DCAST<Explorer*>(dragDroppable))
    {
        EventEmitter<IEditorDragDropManagerListener>::PropagateToListeners(
                    &IEditorDragDropManagerListener::OnExplorerDragStart,
                    explorer->GetSelectedPath());
    }
}

void EditorDragDropManager::OnDragUpdate(UIDragDroppable *dragDroppable)
{
    if (!dragDroppable) { return; }

    if (Explorer *explorer = DCAST<Explorer*>(dragDroppable))
    {
        EventEmitter<IEditorDragDropManagerListener>::PropagateToListeners(
                    &IEditorDragDropManagerListener::OnExplorerDragUpdate,
                    explorer->GetSelectedPath());
    }
}

void EditorDragDropManager::OnDrop(UIDragDroppable *dragDroppable)
{
    if (!dragDroppable) { return; }

    if (Explorer *explorer = DCAST<Explorer*>(dragDroppable))
    {
        EventEmitter<IEditorDragDropManagerListener>::PropagateToListeners(
                    &IEditorDragDropManagerListener::OnExplorerDrop,
                    explorer->GetSelectedPath());
    }
}

EditorDragDropManager *EditorDragDropManager::GetInstance()
{
    EditorScene *es = EditorSceneManager::GetEditorScene();
    return es ? es->GetEditorDragDropManager() : nullptr;
}

