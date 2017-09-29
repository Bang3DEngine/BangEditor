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
    AddComponent<RectTransform>();
    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();

    UIImageRenderer *background = AddComponent<UIImageRenderer>();
    background->SetTint(Color::LightGray);

    AddComponent<UILayoutElement>();

    UIGameObject *titleCont = GameObjectFactory::CreateUIGameObject();
    p_titleText = titleCont->AddComponent<UITextRenderer>();
    p_titleText->SetContent(title);
    AddChild(titleCont);

    AddChild(GameObjectFactory::CreateGUIHSeparator(LayoutSizeType::Min, 10));
}

EditorUITab::~EditorUITab()
{

}

UITextRenderer *EditorUITab::GetTitleText() const
{
    return p_titleText;
}
