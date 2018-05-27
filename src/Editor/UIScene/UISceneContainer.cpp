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
#include "Bang/TextureFactory.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/SelectionFramebuffer.h"

#include "BangEditor/ScenePlayer.h"
#include "BangEditor/UISceneImage.h"
#include "BangEditor/UISceneToolbar.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditorTextureFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UISceneContainer::UISceneContainer()
{
    SetName("SceneContainer");

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleSize( Vector2::One );

    GameObjectFactory::CreateUIGameObjectInto(this);

    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();
    vl->SetChildrenHorizontalStretch(Stretch::FULL);
    vl->SetPaddings(2);
    vl->SetSpacing(3);

    p_sceneToolbar = GameObject::Create<UISceneToolbar>();
    p_sceneImage = GameObject::Create<UISceneImage>();

    GetSceneToolbar()->SetParent(this);
    p_sceneImage->SetParent(this);

    GetSceneToolbar()->EventEmitter<IValueChangedListener>::RegisterListener(this);
    p_sceneImage->GetRectTransform()->
                  EventEmitter<ITransformListener>::RegisterListener(this);
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
            GEngine::GetInstance()->Render(GetContainedScene(), cam);
            OnRenderNeededSceneFinished();
        }
    }
}

void UISceneContainer::SetScene(Scene *scene)
{
    if (GetContainedScene())
    {
        GetContainedScene()->EventEmitter<IDestroyListener>::UnRegisterListener(this);
    }

    p_containedScene = scene;
    if (GetContainedScene())
    {
        GetContainedScene()->EventEmitter<IDestroyListener>::RegisterListener(this);
        p_sceneImage->SetSceneImageCamera( GetSceneCamera(GetContainedScene()) );
    }
}

Scene *UISceneContainer::GetContainedScene() const
{
    return p_containedScene;
}

AARect UISceneContainer::GetSceneImageRectNDC() const
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
    if (containerScene) { containerScene->InvalidateCanvas(); }
}

void UISceneContainer::OnValueChanged(Object*)
{
    p_sceneImage->SetShowDebugStats( GetSceneToolbar()->IsShowDebugStatsChecked() );

    UISceneImage::RenderMode renderMode = SCAST<UISceneImage::RenderMode>(
                  p_sceneToolbar->GetRenderModeComboBox()->GetSelectedValue());
    p_sceneImage->SetRenderMode(renderMode);
}

void UISceneContainer::OnDestroyed(EventEmitter<IDestroyListener>*)
{
    SetScene(nullptr);
}

