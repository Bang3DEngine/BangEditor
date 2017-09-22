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
    m_horizontalLayout->SetSpacing(5);
    m_horizontalLayout->SetChildrenVerticalStretch(Stretch::Full);

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

    MenuBarItem *newProject = m_fileItem->AddChild("New Project...");
    MenuBarItem *saveProject = m_fileItem->AddChild("Save Project");
    MenuBarItem *saveProjectAs = m_fileItem->AddChild("Save Project As...");
    MenuBarItem *openRecentProject = m_fileItem->AddChild("Open Recent Project");
        openRecentProject->AddChild("Wololo/hehe.bproject");
        openRecentProject->AddChild("Wololo/hoho.bproject");
        openRecentProject->AddChild("Wololo/bangbangbang.bproject");
    MenuBarItem *newScene = m_fileItem->AddChild("New Scene");
    MenuBarItem *saveScene = m_fileItem->AddChild("Save Scene");
    MenuBarItem *saveSceneAs= m_fileItem->AddChild("Save Scene As...");

    MenuBarItem *copyChild = m_editItem->AddChild("Copy");
}

MenuBar::~MenuBar()
{
}

MenuBarItem* MenuBar::AddItem()
{
    MenuBarItem *item = new MenuBarItem(false);
    item->SetParent(this);
    m_items.PushBack(item);
    return item;
}

MenuBarItem* MenuBar::GetItem(int i)
{
    return m_items[i];
}
