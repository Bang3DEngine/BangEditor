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

#include "BangEditor/EditorScene.h"
#include "BangEditor/InspectorWidget.h"
#include "BangEditor/ComponentInspectorWidget.h"
#include "BangEditor/FileInspectorWidgetFactory.h"
#include "BangEditor/ComponentInspectorWidgetFactory.h"

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

    // GameObject *topSpacer = GameObjectFactory::CreateUISpacer(LayoutSizeType::Min,
    //                                                           Vector2i(0, 30));

    GameObject *mainVLGo = GameObjectFactory::CreateUIGameObjectNamed("MainVL");
    mainVLGo->GetRectTransform()->SetPivotPosition( Vector2(-1, 1) );

    UILabel *goNameLabel = GameObjectFactory::CreateUILabel();
    GameObject *goNameLabelGo = goNameLabel->GetGameObject();
    UILayoutElement *goNameLE = goNameLabelGo->GetComponent<UILayoutElement>();
    goNameLE->SetFlexibleHeight(0.0f);
    p_titleText = goNameLabel->GetText();
    p_titleText->SetHorizontalAlign(HorizontalAlignment::Left);

    p_titleSeparator = GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Min, 5);
    p_titleSeparator->SetEnabled(false);

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

    goNameLabel->GetGameObject()->SetParent(this);
    p_titleSeparator->SetParent(this);
    scrollPanel->GetGameObject()->SetParent(this);

    Editor::RegisterListener<IEditorSelectionListener>(this);
}

Inspector::~Inspector()
{

}

void Inspector::OnStart()
{
    EditorUITab::OnStart();
}

void Inspector::Update()
{
    EditorUITab::Update();
}

Object *Inspector::GetCurrentObject() const
{
    return p_currentObject;
}

void Inspector::OnDestroyed(Object *destroyedObject)
{
    Clear();
}

void Inspector::OnExplorerPathSelected(const Path &path)
{
    InspectorWidget *fiw = FileInspectorWidgetFactory::Create(path);
    if (fiw)
    {
        Clear();
        p_titleSeparator->SetEnabled(true);
        p_titleText->SetContent(path.GetNameExt());
        AddWidget(fiw);
    }
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
    if(!go || go->IsWaitingToBeDestroyed()) { return; }

    p_currentObject = go;
    p_titleSeparator->SetEnabled(true);
    p_titleText->SetContent(go->GetName());
    GetCurrentObject()->EventEmitter<IDestroyListener>::RegisterListener(this);

    for (Component *comp : go->GetComponents())
    {
        ComponentInspectorWidget *compWidget =
                ComponentInspectorWidgetFactory::Create(comp);
        if (compWidget)
        {
            AddWidget(compWidget);
        }
    }
}

void Inspector::AddWidget(InspectorWidget *widget)
{
    m_widgets.PushBack(widget);
    widget->SetParent( GetContainer() );
}

void Inspector::RemoveWidget(InspectorWidget *widget)
{
    m_widgets.Remove(widget);
    GameObject::Destroy(widget);
}

void Inspector::Clear()
{
    if (GetCurrentObject())
    {
        p_titleText->SetContent("");
        p_titleSeparator->SetEnabled(false);
        GetCurrentObject()->EventEmitter<IDestroyListener>::
                    UnRegisterListener(this);

        while (!m_widgets.IsEmpty())
        {
            InspectorWidget *widget = m_widgets.Front();
            RemoveWidget(widget);
        }

        p_currentObject = nullptr;
    }
}
