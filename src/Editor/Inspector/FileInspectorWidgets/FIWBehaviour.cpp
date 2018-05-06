#include "BangEditor/FIWBehaviour.h"

#include "Bang/File.h"
#include "Bang/Path.h"
#include "Bang/UILabel.h"
#include "Bang/UIButton.h"
#include "Bang/UIScrollBar.h"
#include "Bang/UIScrollArea.h"
#include "Bang/RectTransform.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIContentSizeFitter.h"

#include "BangEditor/QtProjectManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void FIWBehaviour::Init()
{
    FileInspectorWidget::Init();

    SetTitle("Behaviour");
    SetName("FIWBehaviour");

    GameObject *codeTextGo = GameObjectFactory::CreateUIGameObject();
    p_codeText = codeTextGo->AddComponent<UITextRenderer>();
    p_codeText->SetTextSize(12);
    p_codeText->SetWrapping(false);
    p_codeText->SetTextColor(Color::Black);
    p_codeText->SetVerticalAlign(VerticalAlignment::Top);
    p_codeText->SetHorizontalAlign(HorizontalAlignment::Left);

    GameObject *codeContainer = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *codeContHL = codeContainer->AddComponent<UIHorizontalLayout>();
    codeContHL->SetChildrenHorizontalStretch(Stretch::Full);
    codeContHL->SetChildrenVerticalStretch(Stretch::Full);
    codeContHL->SetPaddings(10);
    codeTextGo->SetParent(codeContainer);

    p_codeText->GetGameObject()->GetRectTransform()->SetPivotPosition(Vector2(-1,1));
    codeContainer->GetRectTransform()->SetPivotPosition(Vector2(-1,1));

    UIContentSizeFitter *csf = codeContainer->AddComponent<UIContentSizeFitter>();
    csf->SetHorizontalSizeType(LayoutSizeType::Preferred);
    csf->SetVerticalSizeType(LayoutSizeType::Preferred);

    UIScrollPanel *codeScrollPanel = GameObjectFactory::CreateUIScrollPanel();
    codeScrollPanel->GetScrollArea()->SetContainedGameObject(codeContainer);
    GameObject *scrollPanelGo = codeScrollPanel->GetGameObject();
    UILayoutElement *spLE = scrollPanelGo->AddComponent<UILayoutElement>();
    spLE->SetFlexibleSize( Vector2::One );

    p_openButton = GameObjectFactory::CreateUIButton("Open in editor");
    p_openButton->GetFocusable()->AddClickedCallback([this](IFocusable*)
    {
        QtProjectManager::OpenBehaviourInQtCreator(GetPath());
    });

    AddLabel("Contents");
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 5);
    AddWidget(codeScrollPanel->GetGameObject(), 200);
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 20);
    AddWidget(p_openButton->GetGameObject());

    SetLabelsWidth(100);
}

FIWBehaviour::FIWBehaviour()
{
}

FIWBehaviour::~FIWBehaviour()
{
}

void FIWBehaviour::UpdateFromFileWhenChanged()
{
    String behaviourContents = File(GetPath()).GetContents();
    p_codeText->SetContent(behaviourContents);
}

