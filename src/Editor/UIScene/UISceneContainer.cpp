#include "BangEditor/UISceneContainer.h"

#include "Bang/Scene.h"
#include "Bang/AARect.h"
#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/UIButton.h"
#include "Bang/Texture2D.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIComboBox.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/TextureFactory.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UILayoutIgnorer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/ScenePlayer.h"
#include "BangEditor/UISceneImage.h"
#include "BangEditor/UISceneToolbar.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/SelectionFramebuffer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UISceneContainer::UISceneContainer()
{
    SetName("SceneContainer");

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleSize( Vector2::One );

    GameObjectFactory::CreateUIGameObjectInto(this);

    p_noCameraOverlay = GameObjectFactory::CreateUIGameObject();

    UIImageRenderer *camOverlayImg = p_noCameraOverlay->AddComponent<UIImageRenderer>();
    camOverlayImg->SetTint(Color::Black);

    UITextRenderer *noCameraText = p_noCameraOverlay->AddComponent<UITextRenderer>();
    noCameraText->SetContent("No scene camera. Please assign one.");
    noCameraText->SetVerticalAlign(VerticalAlignment::CENTER);
    noCameraText->SetHorizontalAlign(HorizontalAlignment::CENTER);
    noCameraText->SetTextColor(Color::White);
    noCameraText->SetTextSize(20);

    p_noCameraOverlay->AddComponent<UIFocusable>();
    p_noCameraOverlay->SetEnabled(false);

    GameObject *vlGo = GameObjectFactory::CreateUIGameObject();
    UIVerticalLayout *vl = vlGo->AddComponent<UIVerticalLayout>();
    vl->SetChildrenHorizontalStretch(Stretch::FULL);
    vl->SetPaddings(2);
    vl->SetSpacing(3);

    p_sceneToolbar = GameObject::Create<UISceneToolbar>();
    p_sceneImage = GameObject::Create<UISceneImage>();

    GetSceneToolbar()->SetParent(vlGo);
    GameObjectFactory::CreateUIVSpacer(LayoutSizeType::PREFERRED, 5)->SetParent(vlGo);
    p_sceneImage->SetParent(vlGo);

    GetSceneToolbar()->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_sceneImage->GetRectTransform()->
                  EventEmitter<IEventsTransform>::RegisterListener(this);

    vlGo->SetParent(this);
    p_noCameraOverlay->SetParent(this);
}

UISceneContainer::~UISceneContainer()
{
}

void UISceneContainer::RenderIfNeeded()
{
    if ( NeedsToRenderScene( GetContainedScene() ) )
    {
        Camera *cam = GetSceneCamera(GetContainedScene());
        if (cam)
        {
            cam->SetRenderSize( Vector2i(GetSceneImage()->GetRectTransform()->
                                         GetViewportAARect().GetSize()) );
            GEngine::GetInstance()->Render(GetContainedScene(), cam);
            OnRenderNeededSceneFinished();
        }
        else
        {
            p_sceneImage->GetSceneImageRenderer()->SetTint(Color::Black);
        }

        p_noCameraOverlay->SetEnabled(cam == nullptr);
    }
}

void UISceneContainer::SetScene(Scene *scene)
{
    if (GetContainedScene())
    {
        GetContainedScene()->EventEmitter<IEventsDestroy>::UnRegisterListener(this);
    }

    p_containedScene = scene;
    if (GetContainedScene())
    {
        GetContainedScene()->EventEmitter<IEventsDestroy>::RegisterListener(this);
        p_sceneImage->SetSceneImageCamera( GetSceneCamera(GetContainedScene()) );
    }
}

Scene *UISceneContainer::GetContainedScene() const
{
    return p_containedScene;
}

AARect UISceneContainer::GetSceneImageAARectNDC() const
{
    return AARect(p_sceneImage->GetRectTransform()->GetViewportAARectNDC());
}

UISceneToolbar *UISceneContainer::GetSceneToolbar() const
{
    return p_sceneToolbar;
}

UISceneImage *UISceneContainer::GetSceneImage() const
{
    return p_sceneImage;
}

void UISceneContainer::OnRenderNeededSceneFinished()
{
}

void UISceneContainer::OnTransformChanged()
{
    Scene *containerScene = GetContainedScene();
    if (containerScene)
    {
        containerScene->InvalidateCanvas();
    }
}

void UISceneContainer::OnValueChanged(EventEmitter<IEventsValueChanged>*)
{
    p_sceneImage->SetShowDebugStats( GetSceneToolbar()->IsShowDebugStatsChecked() );

    UISceneImage::RenderMode renderMode = SCAST<UISceneImage::RenderMode>(
                  p_sceneToolbar->GetRenderModeComboBox()->GetSelectedValue());
    p_sceneImage->SetRenderMode(renderMode);
}

void UISceneContainer::OnDestroyed(EventEmitter<IEventsDestroy>*)
{
    SetScene(nullptr);
}

