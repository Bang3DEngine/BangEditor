#include "BangEditor/EditorUITab.h"

#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorUITab::EditorUITab(const String &title)
{
    SetName(title);

    GameObjectFactory::CreateUIGameObjectInto(this);
    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleSize(Vector2(1));

    GameObject *titleCont = GameObjectFactory::CreateUIGameObject();
    p_titleText = titleCont->AddComponent<UITextRenderer>();
    p_titleText->SetTextSize(16);
    p_titleText->SetContent(title);

    UILayoutElement *titleLE = titleCont->AddComponent<UILayoutElement>();
    titleLE->SetMinHeight(16);

    titleCont->SetParent(this);

    SetAsChild(GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Min, 10));
}

EditorUITab::~EditorUITab()
{

}

UITextRenderer *EditorUITab::GetTitleText() const
{
    return p_titleText;
}
