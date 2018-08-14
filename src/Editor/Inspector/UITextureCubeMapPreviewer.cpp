#include "BangEditor/UITextureCubeMapPreviewer.h"

#include "Bang/Asset.h"
#include "Bang/Input.h"
#include "Bang/Material.h"
#include "Bang/UIFocusable.h"
#include "Bang/RectTransform.h"
#include "Bang/MaterialFactory.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIAspectRatioFitter.h"

#include "BangEditor/EditorPaths.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UITextureCubeMapPreviewer::UITextureCubeMapPreviewer()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    GameObject *imgGoContainer = GameObjectFactory::CreateUIGameObject();

    GameObject *imgGo = GameObjectFactory::CreateUIGameObject();
    imgGo->GetRectTransform()->SetAnchorX( Vector2(0, 0) );
    imgGo->GetRectTransform()->SetAnchorY( Vector2(1, 1) );
    imgGo->GetRectTransform()->SetPivotPosition( Vector2(0, 1) );
    p_previewImgAspectRatioFitter = imgGo->AddComponent<UIAspectRatioFitter>();
    p_previewImgAspectRatioFitter->SetAspectRatioMode(AspectRatioMode::KEEP);
    p_previewImageRenderer = imgGo->AddComponent<UIImageRenderer>();
    p_previewImageRenderer->SetMode(UIImageRenderer::Mode::TEXTURE_INV_UVY);

    Material *previewImageRendererMaterial =
                        MaterialFactory::GetUIImage().Get()->Clone();
    p_previewImageRendererMaterial.Set(previewImageRendererMaterial);
    previewImageRendererMaterial->SetShaderProgram(
                ShaderProgramFactory::Get(
                    ShaderProgramFactory::GetEngineShadersDir().Append("UI.vert"),
                    EditorPaths::GetEditorAssetsDir().Append("Shaders").
                                 Append("UITextureCubeMapPreview.frag")) );
    p_previewImageRenderer->SetMaterial( previewImageRendererMaterial );

    p_focusable = imgGo->AddComponent<UIFocusable>();

    imgGo->SetParent(imgGoContainer);
    imgGoContainer->SetParent(this);
}

UITextureCubeMapPreviewer::~UITextureCubeMapPreviewer()
{
}

void UITextureCubeMapPreviewer::Update()
{
    GameObject::Update();

    Vector2i arSize(1, 1);
    if (GetTextureCubeMap())
    {
        arSize = GetTextureCubeMap()->GetSize();
    }
    p_previewImgAspectRatioFitter->SetAspectRatio(arSize);
    p_previewImgAspectRatioFitter->Invalidate();

    Quaternion rotation = Quaternion::FromEulerAngles(
                            Vector3(-m_rotationRads.y, m_rotationRads.x, 0.0f));
    Matrix4 rotationMatrix = Matrix4::RotateMatrix(rotation);

    GL::Push(GL::Pushable::SHADER_PROGRAM);

    ShaderProgram *sp = p_previewImageRenderer->GetMaterial()->GetShaderProgram();
    sp->Bind();
    sp->SetMatrix4("B_Rotation", rotationMatrix, false);
    sp->SetTextureCubeMap("B_TextureCubeMapToPreview", GetTextureCubeMap(), false);

    GL::Pop(GL::Pushable::SHADER_PROGRAM);

    if (p_focusable->IsBeingPressed())
    {
        InvalidateRenderer();
        m_rotationRads += Vector2(Input::GetMouseDelta()) * 0.01f;
        EventEmitter<IEventsValueChanged>::PropagateToListeners(
                        &IEventsValueChanged::OnValueChanged, this);
    }
}

void UITextureCubeMapPreviewer::InvalidateRenderer()
{
    p_previewImageRenderer->EventEmitter<IEventsRendererChanged>::
        PropagateToListeners(&IEventsRendererChanged::OnRendererChanged,
                             p_previewImageRenderer);
}

void UITextureCubeMapPreviewer::SetTextureCubeMap(TextureCubeMap *texCubeMap)
{
    if (texCubeMap != GetTextureCubeMap())
    {
        p_textureCubeMap.Set(texCubeMap);
        EventEmitter<IEventsValueChanged>::PropagateToListeners(
                        &IEventsValueChanged::OnValueChanged, this);
    }
}

TextureCubeMap *UITextureCubeMapPreviewer::GetTextureCubeMap() const
{
    return p_textureCubeMap.Get();
}