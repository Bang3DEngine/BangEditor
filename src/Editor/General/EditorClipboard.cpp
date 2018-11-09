#include "BangEditor/EditorClipboard.h"

#include "Bang/Component.h"
#include "Bang/ComponentMacros.h"
#include "Bang/GameObject.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"

using namespace Bang;
using namespace BangEditor;

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

void EditorClipboard::CopyPath(const Path &path)
{
    EditorClipboard::GetInstance()->m_copiedPath = path;
}

const Path &EditorClipboard::GetCopiedPath()
{
    return EditorClipboard::GetInstance()->m_copiedPath;
}

bool EditorClipboard::HasCopiedPath()
{
    return !EditorClipboard::GetInstance()->m_copiedPath.IsEmpty();
}

void EditorClipboard::ClearCopiedPath()
{
    EditorClipboard::GetInstance()->m_copiedPath = Path::Empty();
}

EditorClipboard *EditorClipboard::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetEditorClipboard();
}
