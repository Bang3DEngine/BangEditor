#include "BangEditor/AESNode.h"

#include "Bang/Input.h"
#include "Bang/UITheme.h"
#include "Bang/UICanvas.h"
#include "Bang/UIRectMask.h"
#include "Bang/UIFocusable.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/AESTransitionEntry.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

AESNode::AESNode()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    RectTransform *rt = GetRectTransform();
    rt->SetAnchors(-Vector2::One);
    rt->SetMarginRightTop(-m_nodeSize);

    p_focusable = AddComponent<UIFocusable>();
    p_focusable->SetCursorType(Cursor::Type::HAND);
    p_focusable->EventEmitter<IEventsFocus>::RegisterListener(this);

    p_bg = AddComponent<UIImageRenderer>();
    p_bg->SetTint(Color::White);

    p_border = GameObjectFactory::AddInnerBorder(this);

    UITextRenderer *textRend = AddComponent<UITextRenderer>();
    textRend->SetTextColor(Color::Black);
    textRend->SetContent("Wololo");

    constexpr int EntrySize = 20;

    p_inEntry = GameObject::Create<AESTransitionEntry>();
    p_inEntry->GetRectTransform()->SetAnchors( Vector2(-1.0f, 0.0f) );
    p_inEntry->GetRectTransform()->SetPivotPosition( Vector2::Zero );
    p_inEntry->GetRectTransform()->SetSizeFromPivot( Vector2i(EntrySize) );
    p_inEntry->SetParent(this);

    p_outEntry = GameObject::Create<AESTransitionEntry>();
    p_outEntry->GetRectTransform()->SetAnchors( Vector2(1.0f, 0.0f) );
    p_outEntry->GetRectTransform()->SetPivotPosition( Vector2::Zero );
    p_outEntry->GetRectTransform()->SetSizeFromPivot( Vector2i(EntrySize) );
    p_outEntry->SetParent(this);
}

AESNode::~AESNode()
{
}

void AESNode::Update()
{
    GameObject::Update();

    Color nodeColor = Color::White;
    RectTransform *rt = GetRectTransform();
    if (p_focusable->IsBeingPressed())
    {
        Vector2 mouseDelta( Input::GetMouseDelta() );
        Vector3 newLocalPos = rt->GetLocalPosition();
        newLocalPos += Vector3(mouseDelta, 0.0f);
        newLocalPos.z = -0.01f;
        rt->SetLocalPosition(newLocalPos);
        nodeColor = UITheme::GetOverColor();
    }
    else
    {
        if (p_focusable->IsMouseOver())
        {
            nodeColor = UITheme::GetOverColor();
        }
    }
    p_bg->SetTint(nodeColor);
}

UIEventResult AESNode::OnUIEvent(UIFocusable *, const UIEvent &event)
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
