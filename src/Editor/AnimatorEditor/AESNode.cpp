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

#include "BangEditor/AESConnectionPoint.h"

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

    constexpr int CPSize = 20;

    p_inConnectionPoint = GameObject::Create<AESConnectionPoint>(
                                                AESConnectionPoint::Type::IN);
    p_inConnectionPoint->GetRectTransform()->SetAnchors( Vector2(-1.0f, 0.0f) );
    p_inConnectionPoint->GetRectTransform()->SetPivotPosition( Vector2::Zero );
    p_inConnectionPoint->GetRectTransform()->SetSizeFromPivot( Vector2i(CPSize) );
    p_inConnectionPoint->SetNode(this);
    p_inConnectionPoint->SetParent(this);

    p_outConnectionPoint = GameObject::Create<AESConnectionPoint>(
                                                AESConnectionPoint::Type::OUT);
    p_outConnectionPoint->GetRectTransform()->SetAnchors( Vector2(1.0f, 0.0f) );
    p_outConnectionPoint->GetRectTransform()->SetPivotPosition( Vector2::Zero );
    p_outConnectionPoint->GetRectTransform()->SetSizeFromPivot( Vector2i(CPSize) );
    p_outConnectionPoint->SetNode(this);
    p_outConnectionPoint->SetParent(this);

    p_inConnectionPoint->SetSiblingConnectionPoint(p_outConnectionPoint);
    p_outConnectionPoint->SetSiblingConnectionPoint(p_inConnectionPoint);
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
