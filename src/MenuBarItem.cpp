#include "BangEditor/MenuBarItem.h"

#include "Bang/LayoutElement.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"

USING_NAMESPACE_BANG_EDITOR

MenuBarItem::MenuBarItem()
{
    AddComponent<RectTransform>();

    UIImageRenderer *img = AddComponent<UIImageRenderer>();
    img->SetTint(Color::LightGray);
    img->SetTint(Random::GetColorOpaque());

    LayoutElement *le = AddComponent<LayoutElement>();
    le->SetMinWidth(100);
    le->SetPreferredWidth(200);

    m_text = AddComponent<UITextRenderer>();
    GetText()->SetContent("MenuItem");
    GetText()->SetTextSize(12);
}

MenuBarItem::~MenuBarItem()
{

}

UITextRenderer *MenuBarItem::GetText() const
{
    return m_text;
}
