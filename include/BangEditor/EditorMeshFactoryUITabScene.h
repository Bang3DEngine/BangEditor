#ifndef EDITORMESHFACTORYUITABSCENE_H
#define EDITORMESHFACTORYUITABSCENE_H

#include "Bang/Bang.h"
#include "Bang/GameObject.h"
#include "Bang/ResourceHandle.h"
#include "BangEditor/BangEditor.h"

using namespace Bang;
namespace Bang
{
class GBuffer;
class UICanvas;
class Model;
class UIFocusable;
class MeshRenderer;
class UITextRenderer;
class UIImageRenderer;
}
namespace BangEditor
{
class EditorMeshFactoryUITabScene : public GameObject
{
public:
    EditorMeshFactoryUITabScene();
    virtual ~EditorMeshFactoryUITabScene();

    // GameObject
    void Update() override;

    void ResetCamera();
    Mesh *GetCurrentMesh() const;
    Model *GetCurrentModel() const;

private:
    UIFocusable *p_focusable = nullptr;
    UIImageRenderer *p_sceneImg = nullptr;
    UITextRenderer *p_centralText = nullptr;

    Scene *p_scene = nullptr;
    RH<Mesh> p_currentMesh;
    RH<Model> p_currentModel;
    GameObject *p_modelContainer = nullptr;

    Camera *p_sceneCamera = nullptr;
    float m_currentCameraZoom = 0.0f;
    Vector2 m_currentCameraRotAngles = Vector2::Zero();

    RH<Mesh> GetExplorerSelectedMesh() const;
    RH<Model> GetExplorerSelectedModel() const;
};
}

#endif  // EDITORMESHFACTORYUITABSCENE_H
