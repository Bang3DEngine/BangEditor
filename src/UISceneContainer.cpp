#include "BangEditor/UISceneContainer.h"

#include "Bang/Rect.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG_EDITOR

UISceneContainer::UISceneContainer()
{
    SetName("SceneContainer");

    GameObjectFactory::CreateUIGameObjectInto(this);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinSize( Vector2i(100) );
    le->SetFlexibleSize( Vector2(6, 1) );

    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();
    vl->SetPaddings(10);

    GameObject *sceneImgGo = GameObjectFactory::CreateUIGameObject();
    p_sceneImg  = sceneImgGo->AddComponent<UIImageRenderer>();
    p_sceneImg->SetUvMultiply(Vector2(1, -1));

    UILayoutElement *imgLE = sceneImgGo->AddComponent<UILayoutElement>();
    imgLE->SetFlexibleSize( Vector2(1) );

    sceneImgGo->SetParent(this);
}

UISceneContainer::~UISceneContainer()
{

}

Rect UISceneContainer::GetImageScreenRectNDC() const
{
    return p_sceneImg->GetGameObject()->GetRectTransform()->GetViewportRectNDC();
}

void UISceneContainer::SetSceneImageTexture(Texture2D *texture)
{
    p_sceneImg->SetImageTexture(texture);
    if (texture) { texture->SetWrapMode(GL::WrapMode::Repeat); }
    p_sceneImg->SetTint(texture ? Color::White : Color::Black);
}
