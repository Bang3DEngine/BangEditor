#ifndef EDITORMESHFACTORYUITABSCENE_H
#define EDITORMESHFACTORYUITABSCENE_H

#include "Bang/AssetHandle.h"
#include "Bang/Bang.h"
#include "Bang/GameObject.h"
#include "BangMath/Plane.h"
#include "BangMath/Ray.h"
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
    AH<Mesh> p_currentMesh;
    AH<Model> p_currentModel;
    GameObject *p_modelContainer = nullptr;

    bool m_displacingModel = false;
    Plane m_displacementPlane;
    Vector3 m_lastModelDisplacementPoint;

    Camera *p_sceneCamera = nullptr;
    float m_currentCameraZoom = 0.0f;
    Vector3 m_cameraOrbitPoint = Vector3::Zero();
    Vector2 m_currentCameraRotAngles = Vector2::Zero();

    void DoCollisionSimulation();

    Ray GetMouseRay() const;
    Vector2 GetMouseNDC() const;
    Vector3 GetDisplacementPoint() const;
    Vector3 GetMousePointOverModel() const;
    Array<MeshRenderer *> GetMeshRenderers() const;
    AH<Mesh> GetExplorerSelectedMesh() const;
    AH<Model> GetExplorerSelectedModel() const;
};
}

#endif  // EDITORMESHFACTORYUITABSCENE_H
