#include "BangEditor/UISceneContainer.h"

#include "Bang/Alignment.h"
#include "Bang/Camera.h"
#include "Bang/Color.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/GEngine.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsTransform.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/RectTransform.h"
#include "Bang/Scene.h"
#include "Bang/Stretch.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/Vector2.h"
#include "BangEditor/UISceneImage.h"
#include "BangEditor/UISceneToolbar.h"
#include "BangEditor/UISceneToolbarDown.h"

namespace Bang
{
class IEventsDestroy;
}

using namespace Bang;
using namespace BangEditor;

UISceneContainer::UISceneContainer()
{
    SetName("SceneContainer");

    GameObjectFactory::CreateUIGameObjectInto(this);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinSize(Vector2i(400));
    le->SetFlexibleSize(Vector2::One);

    UIVerticalLayout *mainVL = AddComponent<UIVerticalLayout>();
    mainVL->SetChildrenHorizontalStretch(Stretch::FULL);

    p_noCameraOverlay = GameObjectFactory::CreateUIGameObject();

    UIImageRenderer *camOverlayImg =
        p_noCameraOverlay->AddComponent<UIImageRenderer>();
    camOverlayImg->SetTint(Color::Black);

    UITextRenderer *noCameraText =
        p_noCameraOverlay->AddComponent<UITextRenderer>();
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

    p_sceneToolbar = new UISceneToolbar();
    p_sceneImage = new UISceneImage();
    p_sceneToolbarDown = new UISceneToolbarDown();

    GetSceneToolbar()->SetParent(vlGo);
    GameObjectFactory::CreateUIVSpacer(LayoutSizeType::PREFERRED, 5)
        ->SetParent(vlGo);
    p_sceneImage->SetParent(vlGo);
    GameObjectFactory::CreateUIVSpacer(LayoutSizeType::PREFERRED, 5)
        ->SetParent(vlGo);
    GetSceneToolbarDown()->SetParent(vlGo);

    p_focusable = AddComponent<UIFocusable>();
    p_focusable->EventEmitter<IEventsFocus>::RegisterListener(this);
    p_focusable->SetConsiderForTabbing(true);

    p_border = GameObjectFactory::AddOuterShadow(GetSceneImage(), Vector2i(2));

    GetSceneToolbar()->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    p_sceneImage->GetRectTransform()
        ->EventEmitter<IEventsTransform>::RegisterListener(this);

    vlGo->SetParent(this);
    p_noCameraOverlay->SetParent(this);
}

UISceneContainer::~UISceneContainer()
{
}

void UISceneContainer::BeforeChildrenRender(RenderPass rp)
{
    if (rp == RenderPass::CANVAS)
    {
        RenderContainedSceneIfNeeded();

        bool showSceneImg = false;
        if (Camera *cam = GetSceneCamera(GetContainedScene()))
        {
            Vector2i sceneImgSize(p_sceneImage->GetRectTransform()
                                      ->GetViewportAARect()
                                      .GetSize());
            bool renderSizeMatches = (sceneImgSize == cam->GetRenderSize());
            showSceneImg = renderSizeMatches;
        }
        p_sceneImage->GetSceneImageRenderer()->SetTint(
            showSceneImg ? Color::Blue : Color::Black);
    }

    GameObject::BeforeChildrenRender(rp);
}

void UISceneContainer::RenderContainedSceneIfNeeded()
{
    if (NeedsToRenderContainedScene(GetContainedScene()))
    {
        if (Camera *cam = GetSceneCamera(GetContainedScene()))
        {
            cam->SetRenderSize(Vector2i(GetSceneImage()
                                            ->GetRectTransform()
                                            ->GetViewportAARect()
                                            .GetSize()));
            OnRenderContainedSceneBegin();
            GEngine::GetInstance()->Render(GetContainedScene(), cam);
            OnRenderContainedSceneFinished();
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
        GetContainedScene()->EventEmitter<IEventsDestroy>::UnRegisterListener(
            this);
    }

    p_containedScene = scene;
    if (GetContainedScene())
    {
        GetContainedScene()->EventEmitter<IEventsDestroy>::RegisterListener(
            this);
        p_sceneImage->SetSceneImageCamera(GetSceneCamera(GetContainedScene()));
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

void UISceneContainer::OnTransformChanged()
{
    Scene *containerScene = GetContainedScene();
    if (containerScene)
    {
        containerScene->InvalidateCanvas();
    }
}

void UISceneContainer::OnValueChanged(EventEmitter<IEventsValueChanged> *)
{
    p_sceneImage->SetShowDebugStats(
        GetSceneToolbar()->IsShowDebugStatsChecked());

    UISceneImage::RenderMode renderMode = SCAST<UISceneImage::RenderMode>(
        p_sceneToolbar->GetRenderModeComboBox()->GetSelectedValue());
    p_sceneImage->SetRenderMode(renderMode);
}

void UISceneContainer::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    GameObject::OnDestroyed(object);

    SetScene(nullptr);
}

UIEventResult UISceneContainer::OnUIEvent(UIFocusable *, const UIEvent &event)
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

        default: break;
    }

    return UIEventResult::IGNORE;
}

void UISceneContainer::OnRenderContainedSceneBegin()
{
}

void UISceneContainer::OnRenderContainedSceneFinished()
{
}
