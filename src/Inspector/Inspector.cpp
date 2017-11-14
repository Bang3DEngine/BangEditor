#include "BangEditor/Inspector.h"

#include "Bang/GL.h"
#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/XMLNode.h"
#include "Bang/UILabel.h"
#include "Bang/Material.h"
#include "Bang/Selection.h"
#include "Bang/GLUniforms.h"
#include "Bang/SceneManager.h"
#include "Bang/UIScrollArea.h"
#include "Bang/RectTransform.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIContentSizeFitter.h"

#include "BangEditor/CWTransform.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/InspectorWidget.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Inspector::Inspector() : EditorUITab("Inspector")
{
    UILayoutElement *le = GetComponent<UILayoutElement>();
    le->SetMinSize( Vector2i(100) );
    le->SetPreferredSize( Vector2i(200) );
    le->SetFlexibleSize( Vector2(1) );

    UIScrollPanel *scrollPanel = GameObjectFactory::CreateUIScrollPanel();
    scrollPanel->GetScrollArea()->GetBackground()->SetVisible(false);

    UILayoutElement *scrollLE = scrollPanel->GetGameObject()->
                                AddComponent<UILayoutElement>();
    scrollLE->SetMinSize( Vector2i::Zero );
    scrollLE->SetPreferredSize( Vector2i(100, 100) );
    scrollLE->SetFlexibleSize( Vector2(1) );

    GameObject *topSpacer =
            GameObjectFactory::CreateUISpacer(LayoutSizeType::Min,
                                              Vector2i(0, 30));

    GameObject *mainVLGo = GameObjectFactory::CreateUIGameObjectNamed("MainVL");
    mainVLGo->GetComponent<RectTransform>()->SetPivotPosition( Vector2(-1, 1) );

    UILabel *goNameLabel = GameObjectFactory::CreateUILabel();
    GameObject *goNameLabelGo = goNameLabel->GetGameObject();
    UILayoutElement *goNameLE = goNameLabelGo->GetComponent<UILayoutElement>();
    goNameLE->SetFlexibleHeight(0.0f);
    p_goNameText = goNameLabel->GetText();
    p_goNameText->SetHorizontalAlign(HorizontalAlignment::Left);

    p_nameSeparator = GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Min, 5);

    UIVerticalLayout *mainVL = mainVLGo->AddComponent<UIVerticalLayout>();
    mainVL->SetSpacing(5);

    UIContentSizeFitter *vlCSF = mainVLGo->AddComponent<UIContentSizeFitter>();
    vlCSF->SetVerticalSizeType(LayoutSizeType::Preferred);

    p_mainVL = mainVL;
    p_scrollPanel = scrollPanel;

    GetScrollPanel()->SetHorizontalScrollEnabled(false);
    GetScrollPanel()->SetVerticalScrollBarSide(HorizontalSide::Right);
    GetScrollPanel()->GetScrollArea()->SetContainedGameObject(
                                                GetMainVL()->GetGameObject() );
    GetScrollPanel()->SetVerticalShowScrollMode(ShowScrollMode::WhenNeeded);

    SetAsChild(goNameLabel->GetGameObject());
    SetAsChild(p_nameSeparator);
    SetAsChild(scrollPanel->GetGameObject());
}

Inspector::~Inspector()
{

}

void Inspector::OnStart()
{
    Object::OnStart();
    Editor::RegisterListener<IEditorSelectionListener>(this);
}

void Inspector::Update()
{
    EditorUITab::Update();
}

void Inspector::OnGameObjectSelected(GameObject *selectedGameObject)
{
    SetGameObject(selectedGameObject);
}

GameObject *Inspector::GetContainer() const
{
    return GetScrollPanel()->GetScrollArea()->GetContainedGameObject();
}
UIVerticalLayout *Inspector::GetMainVL() const { return p_mainVL; }
UIScrollPanel* Inspector::GetScrollPanel() const { return p_scrollPanel; }

void Inspector::SetGameObject(GameObject *go)
{
    Clear();
    ENSURE(go);

    p_nameSeparator->SetEnabled(true);
    p_goNameText->SetContent(go->GetName());
    CWTransform *cwTransform =
                           GameObject::Create<CWTransform>(go->GetTransform());
    AddWidget(cwTransform);
}

void Inspector::AddWidget(InspectorWidget *widget)
{
    m_widgets.PushBack(widget);
    GetContainer()->SetAsChild(widget);
}

void Inspector::RemoveWidget(InspectorWidget *widget)
{
    m_widgets.Remove(widget);
    GameObject::Destroy(widget);
}

void Inspector::Clear()
{
    while (!m_widgets.IsEmpty())
    {
        InspectorWidget *widget = m_widgets.Front();
        p_goNameText->SetContent("");
        p_nameSeparator->SetEnabled(false);
        RemoveWidget(widget);
    }
}
