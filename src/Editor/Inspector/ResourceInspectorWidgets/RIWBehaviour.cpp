#include "BangEditor/RIWBehaviour.h"

#include "Bang/Alignment.h"
#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/Extensions.h"
#include "Bang/File.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/RectTransform.h"
#include "Bang/Stretch.h"
#include "Bang/UIButton.h"
#include "Bang/UIContentSizeFitter.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/UITextRenderer.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/QtProjectManager.h"

namespace Bang
{
class Texture2D;
}

using namespace Bang;
using namespace BangEditor;

void RIWBehaviour::Init()
{
    ResourceInspectorWidget::Init();

    SetTitle("Behaviour");
    SetName("RIWBehaviour");

    GameObject *codeTextGo = GameObjectFactory::CreateUIGameObject();
    p_codeText = codeTextGo->AddComponent<UITextRenderer>();
    p_codeText->SetTextSize(12);
    p_codeText->SetWrapping(false);
    p_codeText->SetTextColor(Color::Black());
    p_codeText->SetVerticalAlign(VerticalAlignment::TOP);
    p_codeText->SetHorizontalAlign(HorizontalAlignment::LEFT);

    GameObject *codeContainer = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *codeContHL =
        codeContainer->AddComponent<UIHorizontalLayout>();
    codeContHL->SetChildrenHorizontalStretch(Stretch::FULL);
    codeContHL->SetChildrenVerticalStretch(Stretch::FULL);
    codeContHL->SetPaddings(10);
    codeTextGo->SetParent(codeContainer);

    p_codeText->GetGameObject()->GetRectTransform()->SetPivotPosition(
        Vector2(-1, 1));
    codeContainer->GetRectTransform()->SetPivotPosition(Vector2(-1, 1));

    UIContentSizeFitter *csf =
        codeContainer->AddComponent<UIContentSizeFitter>();
    csf->SetHorizontalSizeType(LayoutSizeType::PREFERRED);
    csf->SetVerticalSizeType(LayoutSizeType::PREFERRED);

    UIScrollPanel *codeScrollPanel = GameObjectFactory::CreateUIScrollPanel();
    codeScrollPanel->GetScrollArea()->SetContainedGameObject(codeContainer);
    GameObject *scrollPanelGo = codeScrollPanel->GetGameObject();
    UILayoutElement *spLE = scrollPanelGo->AddComponent<UILayoutElement>();
    spLE->SetFlexibleSize(Vector2::One());

    p_openButton = GameObjectFactory::CreateUIButton("Open in editor");
    p_openButton->AddClickedCallback(
        [this]() { QtProjectManager::OpenBehaviourInQtCreator(GetPath()); });

    AddLabel("Contents");
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 5);
    AddWidget(codeScrollPanel->GetGameObject(), 200);
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 20);
    AddWidget(p_openButton->GetGameObject());

    SetLabelsWidth(100);
}

RIWBehaviour::RIWBehaviour()
{
}

RIWBehaviour::~RIWBehaviour()
{
}

void RIWBehaviour::UpdateFromFileWhenChanged()
{
    String behaviourContents = File(GetPath()).GetContents();
    p_codeText->SetContent(behaviourContents);
}

Texture2D *RIWBehaviour::GetIconTexture() const
{
    return EditorTextureFactory::GetIconForExtension(
        Extensions::GetBehaviourExtensions().Front());
}

Color RIWBehaviour::GetIconTint() const
{
    return EditorTextureFactory::GetExtensionIconTint(
        Extensions::GetBehaviourExtensions().Front());
}
