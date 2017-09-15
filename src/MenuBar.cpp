#include "BangEditor/MenuBar.h"

#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"

USING_NAMESPACE_BANG_EDITOR

MenuBar::MenuBar()
{
    RectTransform *rt = AddComponent<RectTransform>();

    rt->SetAnchors(Vector2(-1.0f, 1.0f), Vector2(1.0f, 1.0f));
    rt->SetMarginBot(-20);

    UIImageRenderer *bg = AddComponent<UIImageRenderer>();
    bg->SetTint(Color::LightGray);

    m_horizontalLayout = AddComponent<UIHorizontalLayout>();

    m_fileItem        = AddItem();
    m_editItem        = AddItem();
    m_assetsItem      = AddItem();
    m_componentsItem  = AddItem();
    m_gameObjectsItem = AddItem();

    m_fileItem->GetText()->SetContent("File");
    m_editItem->GetText()->SetContent("Edit");
    m_assetsItem->GetText()->SetContent("Assets");
    m_componentsItem->GetText()->SetContent("Components");
    m_gameObjectsItem->GetText()->SetContent("GameObjects");
}

MenuBar::~MenuBar()
{
}

MenuBarItem* MenuBar::AddItem()
{
    MenuBarItem *item = new MenuBarItem();
    item->SetParent(this);
    m_items.PushBack(item);
    return item;
}

MenuBarItem* MenuBar::GetItem(int i)
{
    return m_items[i];
}
