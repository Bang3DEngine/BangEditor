#include "BangEditor/EditorClipboard.h"

#include "Bang/Component.h"

#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void EditorClipboard::CopyComponent(Component *component)
{
    EditorClipboard::ClearCopiedComponent();
    EditorClipboard *ec = EditorClipboard::GetInstance();
    ec->m_copiedComponent = component->Clone();
}

Component *EditorClipboard::GetCopiedComponent()
{
    EditorClipboard *ec = EditorClipboard::GetInstance();
    return ec->m_copiedComponent;
}

bool EditorClipboard::HasCopiedComponent()
{
    return (GetCopiedComponent() != nullptr);
}

void EditorClipboard::ClearCopiedComponent()
{
    if (GetCopiedComponent())
    {
        Component::Destroy(GetCopiedComponent());

        EditorClipboard *ec = EditorClipboard::GetInstance();
        ec->m_copiedComponent = nullptr;
    }
}


void EditorClipboard::CopyGameObject(GameObject *gameObject)
{
    EditorClipboard::ClearCopiedGameObject();
    EditorClipboard *ec = EditorClipboard::GetInstance();
    ec->m_copiedGameObject = gameObject->Clone();
}

GameObject *EditorClipboard::GetCopiedGameObject()
{
    EditorClipboard *ec = EditorClipboard::GetInstance();
    return ec->m_copiedGameObject;
}

bool EditorClipboard::HasCopiedGameObject()
{
    return EditorClipboard::GetCopiedGameObject() != nullptr;
}

void EditorClipboard::ClearCopiedGameObject()
{
    if (GetCopiedGameObject())
    {
        GameObject::Destroy(GetCopiedGameObject());

        EditorClipboard *ec = EditorClipboard::GetInstance();
        ec->m_copiedGameObject = nullptr;
    }
}

EditorClipboard *EditorClipboard::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetEditorClipboard();
}
