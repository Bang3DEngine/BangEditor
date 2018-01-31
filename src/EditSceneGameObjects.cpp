#include "BangEditor/EditSceneGameObjects.h"

#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditSceneGameObjects::EditSceneGameObjects()
{
    m_editorCamera = GameObject::Create<EditorCamera>();
    m_selectionGizmosManager = new SelectionGizmosManager();
}

EditSceneGameObjects::~EditSceneGameObjects()
{
    GameObject::Destroy(m_editorCamera);
    delete m_selectionGizmosManager;
}

void EditSceneGameObjects::Update()
{
    GetEditorCamera()->Start();
    GetEditorCamera()->Update();
    GetSelectionGizmosManager()->Update();
}

void EditSceneGameObjects::OnBeginRender(Scene *scene)
{
    GetSelectionGizmosManager()->OnBeginRender(scene);
}

SelectionGizmosManager *EditSceneGameObjects::GetSelectionGizmosManager() const
{
    return m_selectionGizmosManager;
}

EditSceneGameObjects *EditSceneGameObjects::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetEditSceneGameObjects();
}

void EditSceneGameObjects::OnEndRender(Scene *scene)
{
    GetSelectionGizmosManager()->OnEndRender(scene);
}

EditorCamera *EditSceneGameObjects::GetEditorCamera() const
{
    return m_editorCamera;
}

