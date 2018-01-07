#include "BangEditor/UIInputFile.h"

#include "Bang/Dialog.h"
#include "Bang/UILabel.h"
#include "Bang/UIButton.h"
#include "Bang/Texture2D.h"
#include "Bang/UIInputText.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/EditorDialog.h"
#include "BangEditor/EditorIconManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UIInputFile::UIInputFile()
{
    SetName("UIInputFile");
    GameObjectFactory::CreateUIGameObjectInto(this);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleWidth( 1.0f );

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetChildrenVerticalStretch(Stretch::Full);
    hl->SetSpacing(5);

    p_pathInputText = GameObjectFactory::CreateUIInputText();
    p_pathInputText->SetBlocked(true);
    p_pathInputText->GetText()->SetTextSize(11);
    UILayoutElement *pathInputTextLE = p_pathInputText->GetGameObject()->
                                       AddComponent<UILayoutElement>();
    pathInputTextLE->SetFlexibleSize( Vector2(9999.9f) );
    pathInputTextLE->SetLayoutPriority(1);

    p_searchButton = GameObjectFactory::CreateUIButton();
    p_searchButton->GetText()->SetContent("");
    p_searchButton->SetIcon( EditorIconManager::GetLensLittleIcon().Get(),
                             Vector2i(14) );
    p_searchButton->GetFocusable()->AddClickedCallback([this](IFocusable*)
    {
        Path openPath;
        bool accepted;
        EditorDialog::GetAsset("Get Asset...",
                               GetExtensions(),
                               &openPath,
                               &accepted);
        if (accepted) { SetPath(openPath); }
    });

    p_pathInputText->GetGameObject()->SetParent(this);
    p_searchButton->GetGameObject()->SetParent(this);
}

UIInputFile::~UIInputFile()
{
}

void UIInputFile::SetPath(const Path &path)
{
    if (path != GetPath())
    {
        m_path = path;

        String textContent = GetPath().IsFile() ? GetPath().GetNameExt() : "None";
        GetInputText()->GetText()->SetContent(textContent);

        EventEmitter<IValueChangedListener>::PropagateToListeners(
                &IValueChangedListener::OnValueChanged, this);
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


