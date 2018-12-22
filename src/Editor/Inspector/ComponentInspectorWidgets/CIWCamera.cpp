#include "BangEditor/CIWCamera.h"

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Camera.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/Extensions.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEvents.h"
#include "Bang/Path.h"
#include "Bang/AssetHandle.h"
#include "Bang/Assets.h"
#include "Bang/Assets.tcc"
#include "Bang/Scene.h"
#include "Bang/TextureCubeMap.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UISlider.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/UIInputColor.h"
#include "BangEditor/UIInputFile.h"
#include "BangEditor/UndoRedoManager.h"
#include "BangEditor/UndoRedoObjectProperty.h"
#include "BangEditor/UndoRedoObjectProperty.tcc"

namespace Bang
{
class IEventsValueChanged;
class Object;
}

using namespace Bang;
using namespace BangEditor;

void CIWCamera::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWCamera");
    SetTitle("Camera");

    p_isActiveCamera = GameObjectFactory::CreateUICheckBox();
    p_isActiveCamera->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    AddWidget("Active Camera", p_isActiveCamera->GetGameObject());

    p_zNearInput = GameObjectFactory::CreateUIInputNumber();
    p_zNearInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    AddWidget("ZNear", p_zNearInput->GetGameObject());

    p_zFarInput = GameObjectFactory::CreateUIInputNumber();
    p_zFarInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    AddWidget("ZFar", p_zFarInput->GetGameObject());

    p_orthoHeightInput = GameObjectFactory::CreateUIInputNumber();
    p_orthoHeightInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    p_orthoHeightInput->SetMinValue(0.0f);
    AddWidget("Ortho Height", p_orthoHeightInput->GetGameObject());

    p_fovInput = GameObjectFactory::CreateUISlider();
    p_fovInput->SetMinMaxValues(0.1f, 180.0f);
    p_fovInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    AddWidget("FOV", p_fovInput->GetGameObject());

    p_projectionModeInput = GameObjectFactory::CreateUIComboBox();
    p_projectionModeInput->AddItem(
        "Orthographic", SCAST<int>(CameraProjectionMode::ORTHOGRAPHIC));
    p_projectionModeInput->AddItem(
        "Perspective", SCAST<int>(CameraProjectionMode::PERSPECTIVE));
    p_projectionModeInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    AddWidget("Projection Mode", p_projectionModeInput->GetGameObject());

    p_clearModeInput = GameObjectFactory::CreateUIComboBox();
    p_clearModeInput->AddItem("Color", SCAST<int>(CameraClearMode::COLOR));
    p_clearModeInput->AddItem("SkyBox", SCAST<int>(CameraClearMode::SKY_BOX));
    p_clearModeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    AddWidget("Clear Mode", p_clearModeInput->GetGameObject());

    p_clearColorInput = new UIInputColor();
    p_clearColorInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    AddWidget("Clear Color", p_clearColorInput);

    p_textureCubeMapInput = new UIInputFile();
    p_textureCubeMapInput->SetExtensions(
        {Extensions::GetTextureCubeMapExtension()});
    p_textureCubeMapInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    AddWidget("SkyBox", p_textureCubeMapInput);

    SetLabelsWidth(90);
}

void CIWCamera::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    if (Scene *openScene = EditorSceneManager::GetOpenScene())
    {
        p_isActiveCamera->SetChecked(openScene->GetCamera() == GetCamera());
    }
    else
    {
        p_isActiveCamera->SetChecked(false);
    }

    if (!p_zNearInput->HasFocus())
    {
        p_zNearInput->SetValue(GetCamera()->GetZNear());
    }

    if (!p_zFarInput->HasFocus())
    {
        p_zFarInput->SetValue(GetCamera()->GetZFar());
    }

    if (!p_orthoHeightInput->HasFocus())
    {
        p_orthoHeightInput->SetValue(GetCamera()->GetOrthoHeight());
    }

    if (!p_fovInput->HasFocus())
    {
        p_fovInput->SetValue(GetCamera()->GetFovDegrees());
    }

    if (!p_projectionModeInput->HasFocus())
    {
        p_projectionModeInput->SetSelectionByValue(
            SCAST<int>(GetCamera()->GetProjectionMode()));
    }

    if (!p_clearModeInput->HasFocus())
    {
        p_clearModeInput->SetSelectionByValue(
            SCAST<int>(GetCamera()->GetClearMode()));
    }

    if (!p_clearColorInput->HasFocus())
    {
        p_clearColorInput->SetColor(GetCamera()->GetClearColor());
    }

    TextureCubeMap *skyBoxTex = GetCamera()->GetSkyBoxTexture();
    if (skyBoxTex)
    {
        p_textureCubeMapInput->SetPath(skyBoxTex->GetAssetFilepath());
    }

    LimitValues();
}

Camera *CIWCamera::GetCamera() const
{
    return SCAST<Camera *>(GetComponent());
}

void CIWCamera::LimitValues()
{
    p_zNearInput->SetMinMaxValues(0.1f, GetCamera()->GetZFar());
    p_zFarInput->SetMinValue(GetCamera()->GetZNear());
}

void CIWCamera::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

    if (object == p_isActiveCamera)
    {
        if (Scene *openScene = EditorSceneManager::GetOpenScene())
        {
            Camera *prevCam = openScene->GetCamera();

            if (p_isActiveCamera->IsChecked())
            {
                openScene->SetCamera(GetCamera());
            }
            else
            {
                openScene->SetCamera(nullptr);
            }

            UndoRedoObjectProperty<Camera *> *undoRedo =
                new UndoRedoObjectProperty<Camera *>(
                    openScene,
                    prevCam,
                    openScene->GetCamera(),
                    [&](Object *scene, Camera *cam) {
                        SCAST<Scene *>(scene)->SetCamera(cam);
                    });
            UndoRedoManager::PushAction(undoRedo);
        }
    }

    GetCamera()->SetZNear(p_zNearInput->GetValue());
    GetCamera()->SetZFar(p_zFarInput->GetValue());
    GetCamera()->SetOrthoHeight(p_orthoHeightInput->GetValue());
    GetCamera()->SetFovDegrees(p_fovInput->GetValue());
    GetCamera()->SetProjectionMode(
        SCAST<CameraProjectionMode>(p_projectionModeInput->GetSelectedValue()));
    GetCamera()->SetClearMode(
        SCAST<CameraClearMode>(p_clearModeInput->GetSelectedValue()));
    GetCamera()->SetClearColor(p_clearColorInput->GetColor());

    AH<TextureCubeMap> tcmAH;
    if (p_textureCubeMapInput->GetPath().IsFile())
    {
        tcmAH =
            Assets::Load<TextureCubeMap>(p_textureCubeMapInput->GetPath());
    }
    GetCamera()->SetSkyBoxTexture(tcmAH.Get());

    LimitValues();
}
