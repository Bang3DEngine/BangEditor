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
#include "BangEditor/UISceneToolbarDown.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/SelectionFramebuffer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UISceneContainer::UISceneContainer()
{
    SetName("SceneContainer");

    GameObjectFactory::CreateUIGameObjectInto(this);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinSize(Vector2i(400));
    le->SetFlexibleSize( Vector2::One );

    UIVerticalLayout *mainVL = AddComponent<UIVerticalLayout>();
    mainVL->SetChildrenHorizontalStretch(Stretch::FULL);

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

    UILayoutElement *vlLE = vlGo->AddComponent<UILayoutElement>();
    vlLE->SetFlexibleSize(Vector2::One);

    p_sceneToolbar = GameObject::Create<UISceneToolbar>();
    p_sceneImage = GameObject::Create<UISceneImage>();
    p_sceneToolbarDown = GameObject::Create<UISceneToolbarDown>();

    GetSceneToolbar()->SetParent(vlGo);
    GameObjectFactory::CreateUIVSpacer(LayoutSizeType::PREFERRED, 5)->SetParent(vlGo);
    p_sceneImage->SetParent(vlGo);
    GameObjectFactory::CreateUIVSpacer(LayoutSizeType::PREFERRED, 5)->SetParent(vlGo);
    GetSceneToolbarDown()->SetParent(vlGo);

    p_focusable = AddComponent<UIFocusable>();
    p_focusable->EventEmitter<IEventsFocus>::RegisterListener(this);
    p_focusable->SetConsiderForTabbing(true);

    p_border = GameObjectFactory::AddOuterShadow(GetSceneImage(), Vector2i(2));

    GetSceneToolbar()->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_sceneImage->GetRectTransform()->
                  EventEmitter<IEventsTransform>::RegisterListener(this);

    vlGo->SetParent(this);
    p_noCameraOverlay->SetParent(this);
}

UISceneContainer::~UISceneContainer()
{
}

void UISceneContainer::Render(RenderPass rp, bool renderChildren)
{
    if (rp == RenderPass::CANVAS)
    {
        bool showSceneImg = false;
        if (Camera *cam = GetSceneCamera(GetContainedScene()))
        {
            Vector2i sceneImgSize( p_sceneImage->GetRectTransform()->
                                   GetViewportAARect().GetSize() );
            bool renderSizeMatches = (sceneImgSize == cam->GetRenderSize());
            showSceneImg = renderSizeMatches;
        }
        p_sceneImage->GetSceneImageRenderer()->SetTint(
                    showSceneImg ? Color::Blue : Color::Black);
    }

    GameObject::Render(rp, renderChildren);
}

void UISceneContainer::RenderIfNeeded()
{
    if ( NeedsToRenderScene( GetContainedScene() ) )
    {
        if (Camera *cam = GetSceneCamera(GetContainedScene()))
        {
            cam->SetRenderSize( Vector2i(GetSceneImage()->GetRectTransform()->
                                         GetViewportAARect().GetSize()) );
            GEngine::GetInstance()->Render(GetContainedScene(), cam);
            OnRenderNeededSceneFinished();
            p_noCameraOverlay->SetEnabled(false);
        }
        else
        {
            p_sceneImage->GetSceneImageRenderer()->SetTint(Color::Black);
            p_noCameraOverlay->SetEnabled(true);
        }
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

UIFocusable *UISceneContainer::GetFocusable() const
{
    return p_focusable;
}

UISceneToolbarDown *UISceneContainer::GetSceneToolbarDown() const
{
    return p_sceneToolbarDown;
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

UIEventResult UISceneContainer::OnUIEvent(UIFocusable*, const UIEvent &event)
{
    switch (event.type)
    {
        case UIEvent::Type::FOCUS_TAKEN:
            GameObjectFactory::MakeBorderFocused(p_border);
            return UIEventResult::INTERCEPT;
        break;

        case UIEvent::Type::FOCUS_LOST:
            GameObjectFactory::MakeBorderNotFocused(p_border);
            return UIEventResult::INTERCEPT;
        break;

        default:
        break;
    }

    return UIEventResult::IGNORE;
}

