#include "BangEditor/UISceneContainer.h"

#include "Bang/Rect.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/UIButton.h"
#include "Bang/Texture2D.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIComboBox.h"
#include "Bang/IconManager.h"
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
#include "BangEditor/EditorIconManager.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UISceneContainer::UISceneContainer()
{
    SetName("SceneContainer");

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleSize( Vector2::One );

    GameObjectFactory::CreateUIGameObjectInto(this);

    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();
    vl->SetChildrenHorizontalStretch(Stretch::Full);
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
    SetScene( GetContainedScene() );
    if ( NeedsToRenderScene( GetContainedScene() ) )
    {
        Camera *cam = GetSceneCamera(GetContainedScene());
        if (cam) { GEngine::GetActive()->Render(GetContainedScene(), cam); }
    }
}

void UISceneContainer::SetScene(Scene *scene)
{
    p_scene = scene;
    if (GetContainedScene())
    {
        p_sceneImage->SetSceneImageCamera( GetSceneCamera(GetContainedScene()) );
    }
}

Scene *UISceneContainer::GetContainedScene() const
{
    return p_scene;
}

Rect UISceneContainer::GetSceneImageRectNDC() const
{
    return p_sceneImage->GetRectTransform()->GetViewportRectNDC();
}

UISceneToolbar *UISceneContainer::GetSceneToolbar() const
{
    return p_sceneToolbar;
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

