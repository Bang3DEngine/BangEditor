#include "BangEditor/UIInputFile.h"

#include "Bang/Color.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsDragDrop.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/Paths.h"
#include "Bang/Resources.h"
#include "Bang/Stretch.h"
#include "Bang/TextureFactory.h"
#include "Bang/UIButton.h"
#include "Bang/UICanvas.h"
#include "Bang/UIDragDroppable.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIInputText.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UITextRenderer.h"
#include "BangEditor/EditorDialog.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/Explorer.h"
#include "BangEditor/ExplorerItem.h"
#include "BangEditor/Inspector.h"

namespace Bang
{
class Texture2D;
}

using namespace Bang;
using namespace BangEditor;

UIInputFile::UIInputFile()
{
    SetName("UIInputFile");
    GameObjectFactory::CreateUIGameObjectInto(this);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleWidth(1.0f);

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetChildrenVerticalStretch(Stretch::FULL);
    hl->SetSpacing(5);

    p_pathInputText = GameObjectFactory::CreateUIInputText();
    p_pathInputText->SetBlocked(true);
    p_pathInputText->GetText()->SetTextSize(12);
    p_pathInputText->GetBackground()->SetTint(Color::White());
    UILayoutElement *pathInputTextLE =
        p_pathInputText->GetGameObject()->AddComponent<UILayoutElement>();
    pathInputTextLE->SetFlexibleSize(Vector2(9999.9f));
    pathInputTextLE->SetLayoutPriority(1);

    Texture2D *lensIcon = EditorTextureFactory::GetLensLittleIcon();
    p_searchButton = GameObjectFactory::CreateUIButton("", lensIcon);
    p_searchButton->SetIcon(lensIcon, Vector2i(16));
    p_searchButton->AddClickedCallback([this]() {
        Path openPath;
        bool accepted;
        EditorDialog::GetAsset(
            "Get Asset...", GetExtensions(), &openPath, &accepted);
        if (accepted)
        {
            SetPath(openPath);
        }
    });

    Texture2D *rightArrowIcon = TextureFactory::GetRightArrowIcon();
    p_openFileInInspectorButton =
        GameObjectFactory::CreateUIButton("", rightArrowIcon);
    p_openFileInInspectorButton->SetIcon(rightArrowIcon, Vector2i(16));
    p_openFileInInspectorButton->GetIcon()->SetTint(Color::Black());
    p_openFileInInspectorButton->AddClickedCallback([this]() {
        if (!Paths::IsEnginePath(GetPath()))
        {
            Explorer::GetInstance()->SelectPath(GetPath(), true);
        }
        else
        {
            // Dont select in explorer, but just show in inspector
            Inspector::GetActive()->ShowPath(GetPath());
        }
    });

    p_pathInputText->GetGameObject()->SetParent(this);
    p_searchButton->GetGameObject()->SetParent(this);
    p_openFileInInspectorButton->GetGameObject()->SetParent(this);

    SetPath(Path::Empty());
}

UIInputFile::~UIInputFile()
{
}

void UIInputFile::OnDragStarted(EventEmitter<IEventsDragDrop> *dd_)
{
    IEventsDragDrop::OnDragStarted(dd_);
}

void UIInputFile::OnDragUpdate(EventEmitter<IEventsDragDrop> *dd_)
{
    IEventsDragDrop::OnDragUpdate(dd_);

    UIDragDroppable *dragDroppable = DCAST<UIDragDroppable *>(dd_);
    if (ExplorerItem *expItem =
            DCAST<ExplorerItem *>(dragDroppable->GetGameObject()))
    {
        Path draggedPath = expItem->GetPath();
        bool acceptedFileType = draggedPath.HasExtension(GetExtensions());

        Color backgroundColor =
            (acceptedFileType ? Color::Green() : Color::Red());

        UICanvas *canvas = UICanvas::GetActive(this);
        if (acceptedFileType &&
            canvas->IsMouseOver(GetInputText()->GetFocusable()))
        {
            backgroundColor = backgroundColor.WithSaturation(0.3f);
        }
        GetInputText()->GetBackground()->SetTint(backgroundColor);

        Color textColor = (acceptedFileType ? Color::Black() : Color::White());
        GetInputText()->GetText()->SetTextColor(textColor);
    }
}

void UIInputFile::OnDrop(EventEmitter<IEventsDragDrop> *dd_)
{
    IEventsDragDrop::OnDrop(dd_);

    if (UICanvas *canvas = UICanvas::GetActive(this))
    {
        if (UIDragDroppable *dragDroppable = DCAST<UIDragDroppable *>(dd_))
        {
            if (ExplorerItem *expItem =
                    DCAST<ExplorerItem *>(dragDroppable->GetGameObject()))
            {
                if (canvas->IsMouseOver(GetInputText()->GetFocusable()))
                {
                    Path draggedPath = expItem->GetPath();
                    bool acceptedFileType =
                        draggedPath.HasExtension(GetExtensions());
                    if (acceptedFileType)
                    {
                        SetPath(draggedPath);
                    }
                }
            }
        }
    }
    GetInputText()->GetBackground()->SetTint(Color::White());
    GetInputText()->GetText()->SetTextColor(Color::Black());
}

void UIInputFile::SetPath(const Path &path)
{
    if (path != GetPath())
    {
        m_path = path;

        bool pathGood =
            (GetPath().IsFile() || Resources::IsEmbeddedResource(path));
        String textContent = pathGood ? GetPath().GetNameExt() : "None";
        GetInputText()->GetText()->SetContent(textContent);

        p_openFileInInspectorButton->SetBlocked(!pathGood);

        EventEmitter<IEventsValueChanged>::PropagateToListeners(
            &IEventsValueChanged::OnValueChanged, this);
    }
}

void UIInputFile::SetExtensions(const Array<String> &extensions)
{
    m_extensions = extensions;
}

Path UIInputFile::GetPath() const
{
    return m_path;
}

UIInputText *UIInputFile::GetInputText() const
{
    return p_pathInputText;
}

const Array<String> &UIInputFile::GetExtensions() const
{
    return m_extensions;
}
