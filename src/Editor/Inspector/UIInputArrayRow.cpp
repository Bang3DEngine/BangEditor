#include "BangEditor/UIInputArrayRow.h"

#include "Bang/Color.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/TextureFactory.h"
#include "Bang/UIButton.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/UIInputArray.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UIInputArrayRow::UIInputArrayRow()
{
}

UIInputArrayRow::~UIInputArrayRow()
{
}

void UIInputArrayRow::Init(UIInputArray *inputArray)
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetSpacing(10);

    UIButton *removeButton = GameObjectFactory::CreateUIButton("",
                                EditorTextureFactory::GetLessIcon());
    removeButton->GetIcon()->SetTint(Color::Red.WithValue(0.75f));
    removeButton->GetGameObject()->SetParent(this);
    removeButton->AddClickedCallback([this, inputArray]()
    {
        inputArray->RemoveRow(this, true);
    });

    GameObject *moveButtonsContainer = GameObjectFactory::CreateUIGameObject();
    UIVerticalLayout *vl = moveButtonsContainer->AddComponent<UIVerticalLayout>();
    vl->SetSpacing(0);

    auto CreateUpDownButton = [this, inputArray](bool up)
    {
        UIButton *upDownButton = GameObjectFactory::CreateUIButton("",
                                    up ? TextureFactory::GetUpArrowIcon() :
                                         TextureFactory::GetDownArrowIcon());
        upDownButton->AddClickedCallback([this, inputArray, up]()
        {
            inputArray->MoveRow(this, (up ? -1 : 1));
        });
        upDownButton->GetIcon()->SetTint(Color::Black);
        upDownButton->GetLayoutElement()->SetMinHeight(10);
        upDownButton->SetIconSize( Vector2i(8, 10) );
        return upDownButton;
    };

    UIButton *moveUpButton = CreateUpDownButton(true);
    UIButton *moveDownButton = CreateUpDownButton(false);
    moveUpButton->GetGameObject()->SetParent(moveButtonsContainer);
    moveDownButton->GetGameObject()->SetParent(moveButtonsContainer);

    moveButtonsContainer->SetParent(this);
}

void UIInputArrayRow::SetContainedGameObject(GameObject *containedGo)
{
    if (containedGo != GetContainedGameObject())
    {
        if (GetContainedGameObject())
        {
            GetContainedGameObject()->SetParent(nullptr);
        }

        p_containedGameObject = containedGo;
        GetContainedGameObject()->SetParent(this);
    }
}

GameObject *UIInputArrayRow::GetContainedGameObject() const
{
    return p_containedGameObject;
}

