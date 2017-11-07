#include "BangEditor/Explorer.h"

#include "Bang/Paths.h"
#include "Bang/UILabel.h"
#include "Bang/UIGridLayout.h"
#include "Bang/UIButtonDriver.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/EditorScene.h"
#include "BangEditor/IconManager.h"
#include "BangEditor/ExplorerEntry.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Explorer::Explorer() : EditorUITab("Explorer")
{
    UILayoutElement *le = GetComponent<UILayoutElement>();
    le->SetMinSize( Vector2i(100) );
    le->SetPreferredWidth(250);

    GameObject *toolBar = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *toolBarHL = toolBar->AddComponent<UIHorizontalLayout>();
    toolBarHL->SetPaddingLeft(3);
    toolBarHL->SetPaddingRight( toolBarHL->GetPaddingLeft() );
    toolBarHL->SetSpacing(3);

    UIButtonDriver *upBtn = GameObjectFactory::CreateUIButton("", nullptr);
    upBtn->SetIcon(IconManager::GetBackArrowIcon(), Vector2i(15), 0);
    upBtn->GetText()->SetContent("");

    UILayoutElement *toolBarLE = toolBar->AddComponent<UILayoutElement>();
    toolBarLE->SetMinHeight(15);
    toolBarLE->SetFlexibleHeight(0);

    p_directionLabel = GameObjectFactory::CreateUILabel();
    p_directionLabel->GetText()->SetTextSize(9);
    p_directionLabel->GetText()->SetHorizontalAlign(HorizontalAlignment::Right);

    GameObject *dirBar = p_directionLabel->GetGameObject();

    p_entriesContainer = GameObjectFactory::CreateUIGameObject();
    UIGridLayout *gridLayout = p_entriesContainer->AddComponent<UIGridLayout>();
    gridLayout->SetSpacing(10);

    SetCurrentFilepath( Paths::EngineAssets() );

    AddChild(toolBar);
    toolBar->AddChild(upBtn->GetGameObject());
    toolBar->AddChild(dirBar);
    AddChild(GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Min, 5));
    AddChild(p_entriesContainer);
}

Explorer::~Explorer()
{
}

void Explorer::SetCurrentFilepath(const Path &path)
{
    if (GetCurrentPath() != path)
    {
        m_currentPath = path;
        p_directionLabel->GetText()->SetContent(m_currentPath.GetAbsolute());

        Clear();
        List<Path> subPaths = m_currentPath.FindSubPaths(Path::FindFlag::Default);
        for (const Path &subPath : subPaths)
        {
            AddEntry(subPath);
        }
    }
}

const Path &Explorer::GetCurrentPath() const
{
    return m_currentPath;
}

void Explorer::Clear()
{
    for (ExplorerEntry *explorerEntry : p_entries)
    {
        GameObject::Destroy(explorerEntry);
    }
    p_entries.Clear();
}

void Explorer::AddEntry(const Path &entryPath)
{
    ExplorerEntry *explorerEntry = new ExplorerEntry();
    explorerEntry->SetFilepath(entryPath);
    p_entriesContainer->AddChild(explorerEntry);

    p_entries.PushBack(explorerEntry);
}

Explorer *Explorer::GetInstance()
{
    return EditorScene::GetInstance()->GetExplorer();
}
