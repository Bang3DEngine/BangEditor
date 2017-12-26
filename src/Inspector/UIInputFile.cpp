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

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetChildrenVerticalStretch(Stretch::Full);
    hl->SetChildrenHorizontalStretch(Stretch::Full);
    hl->SetSpacing(5);

    p_label = GameObjectFactory::CreateUILabel();
    p_label->GetText()->SetContent("File:");
    p_label->GetText()->SetTextSize(11);
    p_label->GetText()->SetHorizontalAlign(HorizontalAlignment::Left);
    p_label->GetGameObject()->GetComponent<UILayoutElement>()->SetFlexibleWidth(0.0f);

    p_pathInputText = GameObjectFactory::CreateUIInputText();
    p_pathInputText->SetBlocked(true);
    p_pathInputText->GetText()->SetTextSize(11);
    UILayoutElement *pathInputTextLE = p_pathInputText->GetGameObject()->
                                       AddComponent<UILayoutElement>();
    pathInputTextLE->SetFlexibleWidth(1.0f);

    p_searchButton = GameObjectFactory::CreateUIButton();
    p_searchButton->GetText()->SetContent("");
    p_searchButton->SetIcon( EditorIconManager::GetLensLittleIcon().Get(),
                             Vector2i(16) );
    p_searchButton->GetButton()->AddClickedCallback([this](IFocusable*)
    {
        Path openPath = OpenFileDialog();
        if (openPath.IsFile()) { SetPath(openPath); }
    });

    p_label->GetGameObject()->SetParent(this);
    p_pathInputText->GetGameObject()->SetParent(this);
    p_searchButton->GetGameObject()->SetParent(this);
}

UIInputFile::~UIInputFile()
{
}

Path UIInputFile::OpenFileDialog() const
{
    return EditorDialog::GetAsset("Get Asset...", GetExtensions());
}

void UIInputFile::SetPath(const Path &path)
{
    if (path != GetPath())
    {
        m_path = path;
        GetInputText()->GetText()->SetContent( GetPath().GetNameExt() );
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

UILabel *UIInputFile::GetLabel() const
{
    return p_label;
}

UIInputText *UIInputFile::GetInputText() const
{
    return p_pathInputText;
}

const Array<String> &UIInputFile::GetExtensions() const
{
    return m_extensions;
}


