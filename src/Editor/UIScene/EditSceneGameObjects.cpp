#include "BangEditor/EditSceneGameObjects.h"

#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/Scene.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/EditorFloor.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/GizmosManager.h"

using namespace Bang;
using namespace BangEditor;

EditSceneGameObjects::EditSceneGameObjects()
{
    m_editorFloor = new EditorFloor();
    m_editorCamera = new EditorCamera();
    m_gizmosManager = new GizmosManager();
}

EditSceneGameObjects::~EditSceneGameObjects()
{
    GameObject::Destroy(m_editorCamera);
    GameObject::Destroy(m_editorFloor);
    delete m_gizmosManager;
}

void EditSceneGameObjects::Update()
{
    GetEditorFloor()->Start();
    GetEditorCamera()->Start();
    GetEditorFloor()->Update();
    GetEditorCamera()->Update();
    GetGizmosManager()->Update();
}

void EditSceneGameObjects::OnBeginRender(Scene *scene)
{
    GetEditorFloor()->SetParent(scene);
    GetGizmosManager()->OnBeginRender(scene);
}

GizmosManager *EditSceneGameObjects::GetGizmosManager() const
{
    return m_gizmosManager;
}

EditSceneGameObjects *EditSceneGameObjects::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetEditSceneGameObjects();
}

void EditSceneGameObjects::OnEndRender(Scene *scene)
{
    GetEditorFloor()->SetParent(nullptr);
    GetGizmosManager()->OnEndRender(scene);
}

EditorCamera *EditSceneGameObjects::GetEditorCamera() const
{
    return m_editorCamera;
}

EditorFloor *EditSceneGameObjects::GetEditorFloor() const
{
    return m_editorFloor;
}
