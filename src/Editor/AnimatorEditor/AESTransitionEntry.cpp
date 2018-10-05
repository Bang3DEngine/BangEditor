#include "BangEditor/AESTransitionEntry.h"

#include "Bang/Input.h"
#include "Bang/UITheme.h"
#include "Bang/Material.h"
#include "Bang/UICanvas.h"
#include "Bang/UIRectMask.h"
#include "Bang/UIFocusable.h"
#include "Bang/LineRenderer.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "Bang/TextureFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

AESTransitionEntry::AESTransitionEntry()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    p_focusable = AddComponent<UIFocusable>();
    p_focusable->SetCursorType(Cursor::Type::HAND);
    p_focusable->EventEmitter<IEventsFocus>::RegisterListener(this);

    p_bg = AddComponent<UIImageRenderer>();
    p_bg->SetTint(Color::White);

    p_border = GameObjectFactory::AddInnerBorder(this);

    p_transitionLineRenderer = AddComponent<LineRenderer>();
    p_transitionLineRenderer->SetViewProjMode(GL::ViewProjMode::CANVAS);
    p_transitionLineRenderer->GetMaterial()->SetAlbedoColor(Color::Green);
    p_transitionLineRenderer->GetMaterial()->SetRenderPass(RenderPass::CANVAS);
    p_transitionLineRenderer->GetMaterial()->SetLineWidth(5.0f);
    p_transitionLineRenderer->SetPoints({Vector3::Zero, Vector3::Zero});

    GameObject *iconGo = GameObjectFactory::CreateUIGameObject();
    UIImageRenderer *icon = iconGo->AddComponent<UIImageRenderer>();
    icon->SetImageTexture( TextureFactory::GetRightArrowIcon() );
    icon->SetTint(Color::DarkGray);
    iconGo->GetRectTransform()->SetMargins(3);
    iconGo->SetParent(this);
}

AESTransitionEntry::~AESTransitionEntry()
{
}

void AESTransitionEntry::Update()
{
    GameObject::Update();

    Color nodeColor = Color::White;
    if (p_focusable->IsBeingPressed())
    {
        Vector2i mousePosition = Input::GetMousePosition();
        Vector2 linePoint = GetRectTransform()->FromWorldToLocalPoint(
                                        Vector3(mousePosition, 0)).xy();
        p_transitionLineRenderer->SetPoint(1, Vector3(linePoint, -0.01f));
        nodeColor = UITheme::GetSelectedColor();
    }
    else
    {
        p_transitionLineRenderer->SetPoint(1, Vector3::Zero);
        if (p_focusable->IsMouseOver())
        {
            nodeColor = UITheme::GetOverColor();
        }
    }
    p_bg->SetTint(nodeColor);
}

UIEventResult AESTransitionEntry::OnUIEvent(UIFocusable *,
                                            const UIEvent &event)
{
    switch (event.type)
    {
        case UIEvent::Type::STARTED_BEING_PRESSED:
        case UIEvent::Type::MOUSE_CLICK_DOWN:
        case UIEvent::Type::MOUSE_CLICK_UP:
        case UIEvent::Type::MOUSE_ENTER:
        case UIEvent::Type::MOUSE_EXIT:
            return UIEventResult::INTERCEPT;
        break;

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

