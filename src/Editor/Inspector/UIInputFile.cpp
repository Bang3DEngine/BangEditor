#include "BangEditor/UIInputFile.h"

#include "Bang/Paths.h"
#include "Bang/Dialog.h"
#include "Bang/UILabel.h"
#include "Bang/UIButton.h"
#include "Bang/UICanvas.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"
#include "Bang/UIInputText.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/TextureFactory.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIDragDroppable.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/Explorer.h"
#include "BangEditor/Inspector.h"
#include "BangEditor/EditorDialog.h"
#include "BangEditor/ExplorerItem.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/FileInspectorWidgetFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UIInputFile::UIInputFile()
{
    SetName("UIInputFile");
    GameObjectFactory::CreateUIGameObjectInto(this);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleWidth( 1.0f );

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetChildrenVerticalStretch(Stretch::FULL);
    hl->SetSpacing(5);

    p_pathInputText = GameObjectFactory::CreateUIInputText();
    p_pathInputText->SetBlocked(true);
    p_pathInputText->GetText()->SetTextSize(12);
    UILayoutElement *pathInputTextLE = p_pathInputText->GetGameObject()->
                                       AddComponent<UILayoutElement>();
    pathInputTextLE->SetFlexibleSize( Vector2(9999.9f) );
    pathInputTextLE->SetLayoutPriority(1);

    Texture2D *lensIcon = EditorTextureFactory::GetLensLittleIcon();
    p_searchButton = GameObjectFactory::CreateUIButton("", lensIcon);
    p_searchButton->SetIcon(lensIcon, Vector2i(16));
    p_searchButton->AddClickedCallback([this]()
    {
        Path openPath;
        bool accepted;
        EditorDialog::GetAsset("Get Asset...",
                               GetExtensions(),
                               &openPath,
                               &accepted);
        if (accepted) { SetPath(openPath); }
    });

    Texture2D *rightArrowIcon = TextureFactory::GetRightArrowIcon();
    p_openFileInInspectorButton =
                  GameObjectFactory::CreateUIButton("", rightArrowIcon);
    p_openFileInInspectorButton->SetIcon(rightArrowIcon, Vector2i(16));
    p_openFileInInspectorButton->AddClickedCallback( [this]()
    {
        if ( !Paths::IsEnginePath( GetPath() ) )
        {
            Explorer::GetInstance()->SelectPath( GetPath(), true );
        }
        else
        {
            // Dont select in explorer, but just show in inspector
            Inspector::GetActive()->ShowPath( GetPath() );
        }
    });

    p_pathInputText->GetGameObject()->SetParent(this);
    p_searchButton->GetGameObject()->SetParent(this);
    p_openFileInInspectorButton->GetGameObject()->SetParent(this);

    SetPath(Path::Empty);
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

    UIDragDroppable *dragDroppable = DCAST<UIDragDroppable*>(dd_);
    ExplorerItem *expItem = DCAST<ExplorerItem*>(dragDroppable->GetGameObject());
    if (expItem)
    {
        Path draggedPath = expItem->GetPath();
        bool acceptedFileType = draggedPath.HasExtension(GetExtensions());

        Color backgroundColor = (acceptedFileType ? Color::Green : Color::Red);

        UICanvas *canvas = UICanvas::GetActive(this);
        if (acceptedFileType && canvas->IsMouseOver(p_pathInputText))
        {
            backgroundColor = backgroundColor.WithSaturation(0.3f);
        }
        p_pathInputText->GetBackground()->SetTint(backgroundColor);

        Color textColor = (acceptedFileType ? Color::Black : Color::White);
        p_pathInputText->GetText()->SetTextColor(textColor);
    }
}

void UIInputFile::OnDrop(EventEmitter<IEventsDragDrop> *dd_,
                         bool inside)
{
    IEventsDragDrop::OnDrop(dd_, inside);

    UICanvas *canvas = UICanvas::GetActive(this);
    if (UIDragDroppable *dragDroppable = DCAST<UIDragDroppable*>(dd_))
    {
        if (ExplorerItem *expItem = DCAST<ExplorerItem*>(dragDroppable->
                                                         GetGameObject()))
        {
            if (canvas->IsMouseOver(p_pathInputText))
            {
                Path draggedPath = expItem->GetPath();
                bool acceptedFileType = draggedPath.
                                        HasExtension(GetExtensions());
                if (acceptedFileType)
                {
                    SetPath(draggedPath);
                }
            }
        }
    }
    p_pathInputText->GetText()->SetTextColor(Color::Black);
    p_pathInputText->GetBackground()->SetTint(Color::White);
}

void UIInputFile::SetPath(const Path &path)
{
    if (path != GetPath())
    {
        m_path = path;

        bool pathGood = (GetPath().IsFile() ||
                         Resources::IsEmbeddedResource(path));
        String textContent = pathGood ? GetPath().GetNameExt() : "None";
        GetInputText()->GetText()->SetContent(textContent);

        p_openFileInInspectorButton->SetBlocked( !pathGood );

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


