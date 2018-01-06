#include "BangEditor/UISceneContainer.h"

#include "Bang/Rect.h"
#include "Bang/UIFocusable.h"
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

    AddComponent<UIFocusable>();

    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();

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

void UISceneContainer::BeforeChildrenRender(RenderPass renderPass)
{
    GameObject::BeforeChildrenRender(renderPass);

    if (renderPass == RenderPass::Canvas)
    {
        m_prevBlendFactorColorSrc = GL::GetBlendSrcFactorColor();
        m_prevBlendFactorColorDst = GL::GetBlendDstFactorColor();
        m_prevBlendFactorAlphaSrc = GL::GetBlendSrcFactorAlpha();
        m_prevBlendFactorAlphaDst = GL::GetBlendDstFactorAlpha();
        m_wasBlendingEnabled = GL::IsEnabled(GL::Test::Blend);

        GL::Enable(GL::Test::Blend);
        GL::BlendFuncSeparate(GL::BlendFactor::One, GL::BlendFactor::Zero,
                              GL::BlendFactor::Zero, GL::BlendFactor::One);
    }
}

void UISceneContainer::AfterChildrenRender(RenderPass renderPass)
{
    GameObject::AfterChildrenRender(renderPass);

    if (renderPass == RenderPass::Canvas)
    {
        GL::BlendFuncSeparate(m_prevBlendFactorColorSrc, m_prevBlendFactorColorDst,
                              m_prevBlendFactorAlphaSrc, m_prevBlendFactorAlphaDst);
        GL::SetEnabled(GL::Test::Blend, m_wasBlendingEnabled);
    }
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
