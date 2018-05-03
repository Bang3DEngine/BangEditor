#include "BangEditor/EditSceneGameObjects.h"

#include "BangEditor/EditorFloor.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditSceneGameObjects::EditSceneGameObjects()
{
    m_editorFloor = GameObject::Create<EditorFloor>();
    m_editorCamera = GameObject::Create<EditorCamera>();
    m_selectionGizmosManager = new SelectionGizmosManager();
}

EditSceneGameObjects::~EditSceneGameObjects()
{
    GameObject::Destroy(m_editorCamera);
    GameObject::Destroy(m_editorFloor);
    delete m_selectionGizmosManager;
}

void EditSceneGameObjects::Update()
{
    GetEditorFloor()->Start();
    GetEditorCamera()->Start();
    GetEditorFloor()->Update();
    GetEditorCamera()->Update();
    GetSelectionGizmosManager()->Update();
}

void EditSceneGameObjects::OnBeginRender(Scene *scene)
{
    GetEditorFloor()->SetParent(scene);
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
    GetEditorFloor()->SetParent(nullptr);
    GetSelectionGizmosManager()->OnEndRender(scene);
}

EditorCamera *EditSceneGameObjects::GetEditorCamera() const
{
    return m_editorCamera;
}

EditorFloor *EditSceneGameObjects::GetEditorFloor() const
{
    return m_editorFloor;
}

