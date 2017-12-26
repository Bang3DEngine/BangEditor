#include "BangEditor/UIInputColor.h"

#include "Bang/UILabel.h"
#include "Bang/UIButton.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/EditorDialog.h"
#include "BangEditor/EditorIconManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UIInputColor::UIInputColor()
{
    SetName("UIInputColor");
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetChildrenVerticalStretch(Stretch::Full);
    hl->SetChildrenHorizontalStretch(Stretch::Full);
    hl->SetSpacing(5);

    p_label = GameObjectFactory::CreateUILabel();
    p_label->GetText()->SetContent("File:");
    p_label->GetText()->SetTextSize(11);
    p_label->GetText()->SetHorizontalAlign(HorizontalAlignment::Left);
    p_label->GetGameObject()->GetComponent<UILayoutElement>()->SetFlexibleWidth(0.0f);

    p_colorImage = GameObjectFactory::CreateUIImage(Color::Black);
    UILayoutElement *pathInputTextLE = p_colorImage->GetGameObject()->
                                       AddComponent<UILayoutElement>();
    pathInputTextLE->SetFlexibleWidth(1.0f);

    p_searchColorButton = GameObjectFactory::CreateUIButton();
    p_searchColorButton->GetText()->SetContent("");
    p_searchColorButton->SetIcon( EditorIconManager::GetLensLittleIcon().Get(),
                                  Vector2i(16) );
    p_searchColorButton->GetButton()->AddClickedCallback([this](IFocusable*)
    {
        // Color color = EditorDialog::GetAsset("Pick Color...", {}});
    });

    p_label->GetGameObject()->SetParent(this);
    p_colorImage->GetGameObject()->SetParent(this);
    p_searchColorButton->GetGameObject()->SetParent(this);
}

UIInputColor::~UIInputColor()
{
}

