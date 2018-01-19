#include "BangEditor/SelectionGizmo.h"

#include "Bang/Camera.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"

#include "BangEditor/EditorCamera.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void SelectionGizmo::SetReferencedGameObject(GameObject *referencedGameObject)
{
    if (GetReferencedGameObject() != referencedGameObject)
    {
        if (GetReferencedGameObject())
        {
            GetReferencedGameObject()->
                    EventEmitter<IDestroyListener>::UnRegisterListener(this);
        }

        p_referencedGameObject = referencedGameObject;
        if (GetReferencedGameObject())
        {
            GetReferencedGameObject()->
                    EventEmitter<IDestroyListener>::RegisterListener(this);
        }
    }
}

GameObject *SelectionGizmo::GetReferencedGameObject() const
{
    return p_referencedGameObject;
}

float SelectionGizmo::GetScaleFactor() const
{
    GameObject *refGo = GetReferencedGameObject();
    Transform *camT = GetEditorCamera()->GetGameObject()->GetTransform();
    float camDist = Vector3::Distance(refGo->GetTransform()->GetPosition(),
                                      camT->GetPosition());
    return camDist;
}

Camera *SelectionGizmo::GetEditorCamera() const
{
    Camera *editorCamera = EditorCamera::GetInstance()->GetCamera();
    return editorCamera;
}

void SelectionGizmo::OnDestroyed(EventEmitter<IDestroyListener> *object)
{
    ASSERT( !GetReferencedGameObject() || object == GetReferencedGameObject() );
    SetReferencedGameObject(nullptr);
}
