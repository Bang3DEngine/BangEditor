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
class MeshRenderer;
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

private:
    UIImageRenderer *p_sceneImg = nullptr;

    Scene *p_scene = nullptr;
    RH<Mesh> p_lastSelectedMesh;
    MeshRenderer *p_meshContainerRenderer = nullptr;

    Camera *p_sceneCamera = nullptr;
    float m_currentCameraZoom = 1.0f;
    Vector2 m_currentCameraRotAngles = Vector2::Zero();

    RH<Mesh> GetExplorerSelectedMesh() const;
};
}

#endif  // EDITORMESHFACTORYUITABSCENE_H
