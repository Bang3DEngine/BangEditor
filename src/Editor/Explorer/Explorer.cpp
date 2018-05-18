#include "BangEditor/Explorer.h"

#include "Bang/File.h"
#include "Bang/Input.h"
#include "Bang/Paths.h"
#include "Bang/Dialog.h"
#include "Bang/UILabel.h"
#include "Bang/UIButton.h"
#include "Bang/UICanvas.h"
#include "Bang/UISlider.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/UIGridLayout.h"
#include "Bang/UIScrollArea.h"
#include "Bang/RectTransform.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/UITextRenderer.h"
#include "Bang/TextureFactory.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIRendererCacher.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/ImportFilesManager.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIContentSizeFitter.h"

#include "BangEditor/Editor.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/QtProjectManager.h"
#include "BangEditor/SceneOpenerSaver.h"
#include "BangEditor/EditorFileTracker.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/ExplorerItemFactory.h"
#include "BangEditor/EditorTextureFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Explorer::Explorer()
{
    SetName("Explorer");

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleSize( Vector2::One );

    GameObjectFactory::CreateUIGameObjectInto(this);
    UIRendererCacher *rendCacher = GameObjectFactory::CreateUIRendererCacherInto(this);
    GameObject *rendererCacherContainer = rendCacher->GetContainer();

    GameObject *mainVLGo = rendererCacherContainer;
    UIVerticalLayout *mainVL = mainVLGo->AddComponent<UIVerticalLayout>();

    // Tool Bar
    GameObject *toolBar = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *toolBarHL = toolBar->AddComponent<UIHorizontalLayout>();
    toolBarHL->SetPaddingTop(3);
    toolBarHL->SetPaddingBot(3);
    toolBarHL->SetPaddingLeft(3);
    toolBarHL->SetPaddingRight(3);
    toolBarHL->SetSpacing(3);

    constexpr int ToolBarHeight = 30;
    UILayoutElement *toolBarLE = toolBar->AddComponent<UILayoutElement>();
    toolBarLE->SetMinHeight(ToolBarHeight);
    toolBarLE->SetFlexibleHeight(0);

    // Scroll Panel
    p_scrollPanel = GameObjectFactory::CreateUIScrollPanel();
    p_scrollPanel->GetScrollArea()->GetBackground()->SetTint(
                                        Color::LightGray.WithValue(0.9f));
    GameObject *scrollPanelGo = p_scrollPanel->GetGameObject();
    UILayoutElement *spLE = scrollPanelGo->AddComponent<UILayoutElement>();
    spLE->SetFlexibleSize( Vector2::One );

    // Back button
    RH<Texture2D> backButtonTex = EditorTextureFactory::GetBackArrowIcon();
    p_backButton = GameObjectFactory::CreateUIButton("", backButtonTex.Get());
    p_backButton->SetIcon(backButtonTex.Get(), Vector2i(20, 15), 0);
    p_backButton->GetText()->SetContent("");
    p_backButton->GetFocusable()->AddClickedCallback( [this](IFocusable*)
    { GoDirectoryUp(); });

    // Direction label
    p_currentPathLabel = GameObjectFactory::CreateUILabel();
    p_currentPathLabel->GetText()->SetTextSize(11);
    p_currentPathLabel->GetText()->SetHorizontalAlign(HorizontalAlignment::Right);

    GameObject *dirBar = p_currentPathLabel->GetGameObject();

    // Items Container
    p_itemsContainer = GameObjectFactory::CreateUIGameObject();
    p_itemsContainer->GetRectTransform()->SetPivotPosition(Vector2(-1,1));

    UIContentSizeFitter *csf = p_itemsContainer->AddComponent<UIContentSizeFitter>();
    csf->SetHorizontalSizeType(LayoutSizeType::None);
    csf->SetVerticalSizeType(LayoutSizeType::Preferred);

    // Grid layout
    p_explorerGridLayout = p_itemsContainer->AddComponent<UIGridLayout>();
    p_explorerGridLayout->SetPaddings(10);
    p_explorerGridLayout->SetSpacing(10);

    // Icon size slider
    p_iconSizeSlider = GameObjectFactory::CreateUISlider();
    p_iconSizeSlider->GetInputNumber()->SetDecimalPlaces(0);
    p_iconSizeSlider->SetMinMaxValues(64, 256);
    p_iconSizeSlider->GetInputNumber()->GetGameObject()->SetEnabled(false);
    p_iconSizeSlider->EventEmitter<IValueChangedListener>::RegisterListener(this);
    p_iconSizeSlider->SetValue(128);

    UIImageRenderer *eyeImg = GameObjectFactory::CreateUIImage();
    eyeImg->SetImageTexture(EditorTextureFactory::GetEyeIcon().Get());
    UILayoutElement *eyeImgLE = eyeImg->GetGameObject()->
                                        AddComponent<UILayoutElement>();
    eyeImgLE->SetPreferredSize( Vector2i(20, ToolBarHeight) );
    eyeImgLE->SetFlexibleSize( Vector2(0,1) );

    UILayoutElement *iconsSizeSliderLE =
            p_iconSizeSlider->GetGameObject()->AddComponent<UILayoutElement>();
    iconsSizeSliderLE->SetMinWidth(128);
    iconsSizeSliderLE->SetFlexibleWidth(0.0f);
    iconsSizeSliderLE->SetLayoutPriority(2);

    p_backButton->GetGameObject()->SetParent(toolBar);
    GameObjectFactory::CreateUIVSeparator(LayoutSizeType::Min, 15)->SetParent(toolBar);
    eyeImg->GetGameObject()->SetParent(toolBar);
    GameObjectFactory::CreateUIVSpacer(LayoutSizeType::Min, 5)->SetParent(toolBar);
    p_iconSizeSlider->GetGameObject()->SetParent(toolBar);
    dirBar->SetParent(toolBar);
    toolBar->SetParent(mainVLGo);
    GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Min, 5)->SetParent(mainVLGo);
    p_scrollPanel->GetGameObject()->SetParent(mainVLGo);

    p_scrollPanel->GetScrollArea()->SetContainedGameObject(p_itemsContainer);
    p_scrollPanel->SetVerticalShowScrollMode(ShowScrollMode::WhenNeeded);
    p_scrollPanel->SetVerticalScrollBarSide(HorizontalSide::Right);
    p_scrollPanel->SetHorizontalScrollEnabled(false);

    SetCurrentPath( Paths::GetEngineAssetsDir() );

    Editor::GetInstance()->
            EventEmitter<IEditorListener>::RegisterListener(this);
    ProjectManager::GetInstance()->
            EventEmitter<IProjectManagerListener>::RegisterListener(this);
    EditorFileTracker::GetInstance()->GetFileTracker()->
            EventEmitter<IFileTrackerListener>::RegisterListener(this);

    ShortcutManager::RegisterShortcut(Shortcut(Key::LCtrl, Key::D, "Duplicate"),
                                      &Explorer::OnShortcutPressed);
    ShortcutManager::RegisterShortcut(Shortcut(Key::F2, "Rename"),
                                      &Explorer::OnShortcutPressed);
    ShortcutManager::RegisterShortcut(Shortcut(Key::Delete, "Delete"),
                                      &Explorer::OnShortcutPressed);
}

Explorer::~Explorer()
{
}

void Explorer::Update()
{
    GameObject::Update();

    if (Input::GetMouseButtonDown(MouseButton::Left) ||
        Input::GetMouseButtonDown(MouseButton::Right))
    {
        UICanvas *canvas = UICanvas::GetActive(this);
        for (ExplorerItem *explorerItem : p_items)
        {
            if (canvas->IsMouseOver(explorerItem, true))
            {
                SelectPath(explorerItem->GetPath());
                break;
            }
        }
    }

    #ifdef DEBUG
    if (Input::GetKey(Key::P) && Input::GetKey(Key::Num0))
    {
        SetRootPath(EditorPaths::GetEngineAssetsDir());
    }
    #endif
}

void Explorer::ForceCheckFileChanges()
{
    EditorFileTracker::GetInstance()->GetFileTracker()->Update(true);
}

void Explorer::SelectPath(const Path &path)
{
    for (ExplorerItem *explorerItem : p_items)
    {
        explorerItem->SetSelected(false);
    }

    if (path.GetDirectory().Exists()) { SetCurrentPath(path.GetDirectory()); }
    ExplorerItem *explorerItem = GetItemFromPath(path);
    if (explorerItem)
    {
        explorerItem->SetSelected(true);
        Editor::OnPathSelected(explorerItem->GetPath());
    }
}

void Explorer::SetRootPath(const Path &rootPath)
{
    if (rootPath != GetRootPath())
    {
        m_rootPath = rootPath;
        if (!IsInsideRootPath( GetCurrentPath() ))
        {
            SetCurrentPath( GetRootPath() );
        }
    }
}

void Explorer::SetCurrentPath(const Path &path)
{
    if (GetCurrentPath() != path && IsInsideRootPath(path))
    {
        m_currentPath = path;
        p_currentPathLabel->GetText()->SetContent(GetCurrentPath().GetAbsolute());

        p_backButton->SetBlocked( GetCurrentPath() == GetRootPath() );

        Clear();

        List<ExplorerItem*> subExplorerItems =
                ExplorerItemFactory::CreateAndGetChildrenExplorerItems(path);
        for (ExplorerItem *expItem : subExplorerItems)
        {
            AddItem(expItem);
        }
        p_scrollPanel->SetScrolling(Vector2i::Zero);
    }
}

const Path &Explorer::GetRootPath() const
{
    return m_rootPath;
}

const Path &Explorer::GetCurrentPath() const
{
    return m_currentPath;
}

const Path &Explorer::GetSelectedPath() const
{
    ExplorerItem *selectedItem = GetSelectedItem();
    return selectedItem ? selectedItem->GetPath() : Path::Empty;
}

void Explorer::Clear()
{
    while (!p_items.IsEmpty())
    {
        RemoveItem(p_items.Front()->GetPath());
    }
    m_pathsToItem.Clear();
    p_items.Clear();
}

void Explorer::OnProjectOpen(const Project *project)
{
    IProjectManagerListener::OnProjectOpen(project);
    SetRootPath(Paths::GetProjectAssetsDir());
    SetCurrentPath(Paths::GetProjectAssetsDir());
}

void Explorer::OnProjectClosed(const Project *project)
{
    IProjectManagerListener::OnProjectClosed(project);
    SetCurrentPath(Paths::GetEngineAssetsDir());
}

void Explorer::OnPathAdded(const Path &addedPath)
{
    if ( addedPath.GetDirectory() == GetCurrentPath() &&
        !addedPath.IsHiddenFile() &&
         addedPath.Exists())
    {
        AddItem(addedPath);
    }
}

void Explorer::OnPathModified(const Path &)
{
}

void Explorer::OnPathRemoved(const Path &removedPath)
{
    if (!removedPath.IsFile())
    {
        RemoveItem(removedPath);
    }
}

void Explorer::OnGameObjectSelected(GameObject *selectedGameObject)
{
    if (selectedGameObject)
    {
        SelectPath(Path::Empty);
    }
}

void Explorer::AddItem(const Path &itemPath)
{
    AddItem( ExplorerItemFactory::CreateExplorerItem(itemPath) );
}

void Explorer::AddItem(ExplorerItem *explorerItem)
{
    Path itemPath = explorerItem->GetPath();
    if ( GetItemFromPath(itemPath) ) { return; }

    explorerItem->SetParent(p_itemsContainer);

    explorerItem->GetFocusable()->AddDoubleClickedCallback(
                                        &Explorer::OnItemDoubleClicked);

    explorerItem->EventEmitter<IExplorerItemListener>::RegisterListener(this);

    p_items.PushBack(explorerItem);
    m_pathsToItem.Add(itemPath, explorerItem);
}

void Explorer::RemoveItem(const Path &itemPath)
{
    ExplorerItem *explorerItem = GetItemFromPath(itemPath);
    if (explorerItem)
    {
        explorerItem->EventEmitter<IExplorerItemListener>::
                UnRegisterListener(DCAST<IExplorerItemListener*>(this));

        p_items.Remove(explorerItem);
        m_pathsToItem.Remove(itemPath);
        GameObject::Destroy(explorerItem);
    }
}

void Explorer::GoDirectoryUp()
{
    SetCurrentPath( GetCurrentPath().GetDirectory() );
}

void Explorer::OnRename(ExplorerItem *explorerItem)
{
    const Path &path = explorerItem->GetPath();

    String newName = Dialog::GetString("Rename", "Introduce the new name:",
                                       path.GetName());
    String oldExtensions = String::Join(path.GetExtensions(), ".");

    if (!newName.IsEmpty())
    {
        Path newPath = path.GetDirectory().Append(newName);

        String newExtension = newPath.GetExtension();
        if (newExtension.IsEmpty())
        {
            newPath = newPath.AppendExtension(oldExtensions);
        }

        if (newPath != path)
        {
            if (newPath.Exists())
            {
                Dialog::Error("Can't rename",
                              "The path '" + newPath.GetAbsolute() +
                              "' already exists.");
            }
            else
            {
                File::Rename(path, newPath);
                EditorFileTracker::GetInstance()->OnPathRenamed(path, newPath);
                explorerItem->SetPath(newPath);
            }
        }
    }

    ForceCheckFileChanges();
}

void Explorer::OnRemove(ExplorerItem *explorerItem)
{
    const Path &path = explorerItem->GetPath();
    Dialog::YesNoCancel yesNoCancel =
        Dialog::GetYesNoCancel("Remove", "Are you sure you want to remove '" +
                               path.GetNameExt() + "' ?");

    if (yesNoCancel == Dialog::YesNoCancel::Yes)
    {
        File::Remove( path );
        File::Remove( ImportFilesManager::GetImportFilepath(path) );
    }

    ForceCheckFileChanges();
}

void DuplicateImportFiles(const Path &oriPath, const Path &dupPath)
{
    ASSERT(oriPath.Exists() && dupPath.Exists());
    ASSERT(oriPath != dupPath);

    if (dupPath.IsFile())
    {
        ImportFilesManager::DuplicateImportFile(oriPath, dupPath);
    }
    else
    {
        Array<Path> oriSubPaths = oriPath.GetSubPaths(Path::FindFlag::Simple).To<Array>();
        Array<Path> dupSubPaths = dupPath.GetSubPaths(Path::FindFlag::Simple).To<Array>();
        for (uint i = 0; i < oriSubPaths.Size(); ++i)
        {
            DuplicateImportFiles(oriSubPaths[i], dupSubPaths[i]);
        }
    }
}

void Explorer::OnDuplicate(ExplorerItem *explorerItem)
{
    Path path = explorerItem->GetPath();
    Path newPathName = path.GetDuplicatePath();
    File::Duplicate(path, newPathName);

    DuplicateImportFiles(path, newPathName);

    ForceCheckFileChanges();
    Explorer::SelectPath(newPathName);
}

ExplorerItem *Explorer::GetSelectedItem() const
{
    for (ExplorerItem *explorerItem : p_items)
    {
        if (explorerItem->IsSelected()) { return explorerItem; }
    }
    return nullptr;
}

ExplorerItem *Explorer::GetItemFromPath(const Path &path) const
{
    return m_pathsToItem.ContainsKey(path) ? m_pathsToItem.Get(path) : nullptr;
}

void Explorer::OnItemDoubleClicked(IFocusable *itemFocusable)
{
    GameObject *itemGo = SCAST<UIFocusable*>(itemFocusable)->GetGameObject();
    ExplorerItem *expItem = SCAST<ExplorerItem*>(itemGo);
    ASSERT(expItem);

    const Path itemPath = expItem->GetPath();
    if ( ExplorerItemFactory::CanHaveChildren(itemPath) )
    {
        Explorer::GetInstance()->SetCurrentPath(itemPath);
    }
    else
    {
        if (itemPath.HasExtension(Extensions::GetSceneExtension()))
        {
            if (Editor::IsEditingScene())
            {
                SceneOpenerSaver::GetInstance()->OpenSceneInEditor(itemPath);
            }
        }
        else if (itemPath.HasExtension(Extensions::GetBehaviourExtensions()))
        {
            QtProjectManager::OpenBehaviourInQtCreator(itemPath);
        }
    }
}

void Explorer::OnShortcutPressed(const Shortcut &shortcut)
{
    Explorer *exp = Explorer::GetInstance();

    ExplorerItem *selectedItem = exp->GetSelectedItem();
    if (selectedItem)
    {
        if (shortcut.GetName() == "Rename")
        { selectedItem->Rename(); }

        if (shortcut.GetName() == "Duplicate")
        { selectedItem->Duplicate(); }

        if (shortcut.GetName() == "Delete")
        { selectedItem->Remove(); }
    }
}

bool Explorer::IsInsideRootPath(const Path &path) const
{
    return path.GetAbsolute().BeginsWith( GetRootPath().GetAbsolute() );
}

void Explorer::OnValueChanged(Object*)
{
    p_explorerGridLayout->SetCellSize( 
        Vector2i( SCAST<int>( p_iconSizeSlider->GetValue() ) ) );
}

Explorer *Explorer::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetExplorer();
}
