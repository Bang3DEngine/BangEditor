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

    AddComponent<RectTransform>();
    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();

    AddComponent<UILayoutElement>();
    UIImageRenderer *bg = AddComponent<UIImageRenderer>();
    bg->SetTint(Random::GetColorOpaque());

    // UIGameObject *titleCont = GameObjectFactory::CreateUIGameObject();
    // p_titleText = titleCont->AddComponent<UITextRenderer>();
    // p_titleText->SetContent(title);
    // titleCont->SetParent(this);

    // UIGameObject *imgCont = GameObjectFactory::CreateUIGameObject();
    // UIImageRenderer *img = imgCont->AddComponent<UIImageRenderer>();
    // img->SetTint(Color::Blue);
    // imgCont->AddComponent<UILayoutElement>();
    // AddChild(imgCont);

    // AddChild(GameObjectFactory::CreateGUIHSeparator(LayoutSizeType::Min, 10));
}

EditorUITab::~EditorUITab()
{

}

UITextRenderer *EditorUITab::GetTitleText() const
{
    return p_titleText;
}
