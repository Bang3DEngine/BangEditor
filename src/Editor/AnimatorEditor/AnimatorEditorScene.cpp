#include "BangEditor/AnimatorEditorScene.h"

#include "Bang/Input.h"
#include "Bang/UITheme.h"
#include "Bang/UICanvas.h"
#include "Bang/UIRectMask.h"
#include "Bang/UIFocusable.h"
#include "Bang/RectTransform.h"
#include "Bang/TextureFactory.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/AESNode.h"
#include "BangEditor/EditorTextureFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

AnimatorEditorScene::AnimatorEditorScene()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    AddComponent<UIRectMask>();

    UIImageRenderer *nodesSceneBG = AddComponent<UIImageRenderer>();
    nodesSceneBG->SetTint(Color::DarkGray);

    GameObject *borderGo = GameObjectFactory::CreateUIGameObject();
    p_border = GameObjectFactory::AddInnerBorder(borderGo);

    p_focusable = AddComponent<UIFocusable>();
    p_focusable->EventEmitter<IEventsFocus>::RegisterListener(this);

    p_mainContainer = GameObjectFactory::CreateUIGameObject();
    p_mainContainer->SetParent(this);

    AESNode *node = GameObject::Create<AESNode>();
    node->GetRectTransform()->SetLocalPosition( Vector3(50.0f, 50.0f, 0.0f) );
    node->SetParent(p_mainContainer);

    AESNode *node2 = GameObject::Create<AESNode>();
    node2->GetRectTransform()->SetLocalPosition( Vector3(200.0f, 400.0f, 0.0f) );
    node2->SetParent(p_mainContainer);

    borderGo->SetParent(this);
}

AnimatorEditorScene::~AnimatorEditorScene()
{
}

void AnimatorEditorScene::Update()
{
    GameObject::Update();

    if (UICanvas::GetActive(this)->HasFocus(this, true))
    {
        if (Input::GetMouseButton(MouseButton::MIDDLE))
        {
            Vector2i mouseDelta = Input::GetMouseDelta();
            m_panning += mouseDelta;

            RectTransform *mainContRT = p_mainContainer->GetRectTransform();
            mainContRT->SetLocalPosition( Vector3(Vector2(m_panning), 0.0f) );
        }

        Vector2 mouseWheel = (Input::GetMouseWheel() * Vector2(0.015f));
        if (mouseWheel != Vector2::Zero)
        {
            m_zoomScale = Math::Clamp(m_zoomScale + mouseWheel.y, 0.1f, 2.0f);
        }
    }

    p_mainContainer->GetRectTransform()->SetLocalScale(
                     Vector3(Vector2(m_zoomScale), 1.0f) );
}

UIEventResult AnimatorEditorScene::OnUIEvent(UIFocusable *, const UIEvent &event)
{
    switch (event.type)
    {
        case UIEvent::Type::FOCUS_LOST:
            GameObjectFactory::MakeBorderNotFocused(p_border);
            return UIEventResult::INTERCEPT;
        break;

        case UIEvent::Type::FOCUS_TAKEN:
            GameObjectFactory::MakeBorderFocused(p_border);
            return UIEventResult::INTERCEPT;
        break;

        default:
        break;
    }
    return UIEventResult::IGNORE;
}

