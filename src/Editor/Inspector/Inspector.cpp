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
#include "Bang/UIRendererCacher.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIContentSizeFitter.h"

#include "BangEditor/EditorScene.h"
#include "BangEditor/InspectorWidget.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/ComponentInspectorWidget.h"
#include "BangEditor/FileInspectorWidgetFactory.h"
#include "BangEditor/ComponentInspectorWidgetFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Inspector::Inspector()
{
    SetName("Inspector");

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinWidth(200);
    le->SetPreferredWidth(400);
    le->SetFlexibleSize( Vector2::One );

    GameObjectFactory::CreateUIGameObjectInto(this);
    UIRendererCacher *rendCacher = GameObjectFactory::CreateUIRendererCacherInto(this);
    GameObject *rendererCacherContainer = rendCacher->GetContainer();
    rendCacher->GetContainer()->GetRectTransform()->SetMargins(0, 10, 0, 5);

    GameObject *mainVLGo = rendererCacherContainer;
    UIVerticalLayout *mainVL = mainVLGo->AddComponent<UIVerticalLayout>(); (void)(mainVL);
    UILayoutElement *mainVLLE = mainVLGo->AddComponent<UILayoutElement>();
    mainVLLE->SetFlexibleSize(Vector2::One);

    UIScrollPanel *scrollPanel = GameObjectFactory::CreateUIScrollPanel();
    scrollPanel->GetScrollArea()->GetBackground()->SetVisible(false);

    UILayoutElement *scrollLE = scrollPanel->GetGameObject()->
                                AddComponent<UILayoutElement>();
    scrollLE->SetFlexibleSize( Vector2::One );

    // GameObject *topSpacer = GameObjectFactory::CreateUISpacer(LayoutSizeType::Min,
    //                                                           Vector2i(0, 30));

    GameObject *widgetsVLGo = GameObjectFactory::CreateUIGameObjectNamed("MainVL");
    widgetsVLGo->GetRectTransform()->SetPivotPosition( Vector2(-1, 1) );
    widgetsVLGo->SetParent(rendererCacherContainer);

    UILabel *goNameLabel = GameObjectFactory::CreateUILabel();
    GameObject *goNameLabelGo = goNameLabel->GetGameObject();
    UILayoutElement *goNameLE = goNameLabelGo->GetComponent<UILayoutElement>();
    goNameLE->SetFlexibleHeight(0.0f);
    p_titleText = goNameLabel->GetText();
    p_titleText->SetHorizontalAlign(HorizontalAlignment::Left);

    p_titleSeparator = GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Min, 5);
    p_titleSeparator->SetEnabled(false);

    UIVerticalLayout *widgetsVL = widgetsVLGo->AddComponent<UIVerticalLayout>();
    widgetsVL->SetSpacing(10);
    widgetsVL->SetPaddingTop(10);
    widgetsVL->SetPaddingLeft(5);
    widgetsVL->SetPaddingRight(10);

    UIContentSizeFitter *vlCSF = widgetsVLGo->AddComponent<UIContentSizeFitter>();
    vlCSF->SetVerticalSizeType(LayoutSizeType::Preferred);

    p_mainVL = widgetsVL;
    p_scrollPanel = scrollPanel;

    GetScrollPanel()->SetHorizontalScrollEnabled(false);
    GetScrollPanel()->SetVerticalScrollBarSide(HorizontalSide::Right);
    GetScrollPanel()->GetScrollArea()->SetContainedGameObject(
                                                GetMainVL()->GetGameObject() );
    GetScrollPanel()->SetVerticalShowScrollMode(ShowScrollMode::WhenNeeded);

    goNameLabel->GetGameObject()->SetParent(mainVLGo);
    p_titleSeparator->SetParent(mainVLGo);
    scrollPanel->GetGameObject()->SetParent(mainVLGo);

    // Add a bit of margin below...
    GameObjectFactory::CreateUIVSpacer(LayoutSizeType::Min, 40)->SetParent(
                                                               GetWidgetsContainer());

    Editor::GetInstance()->EventEmitter<IEditorListener>::RegisterListener(this);
    SceneManager::GetActive()->EventEmitter<ISceneManagerListener>::RegisterListener(this);
}

Inspector::~Inspector()
{

}

void Inspector::OnStart()
{
    GameObject::OnStart();
}

void Inspector::Update()
{
    GameObject::Update();
}

GameObject *Inspector::GetCurrentGameObject() const
{
    return p_currentGameObject;
}

void Inspector::OnSceneLoaded(Scene*, const Path &)
{
    Clear();
}

void Inspector::OnDestroyed(EventEmitter<IDestroyListener>*)
{
    Clear();
}

void Inspector::OnExplorerPathSelected(const Path &path)
{
    InspectorWidget *fiw = FileInspectorWidgetFactory::Create(path);
    if (fiw || path.IsFile()) { Clear(); }
    if (fiw)
    {
        p_titleSeparator->SetEnabled(true);
        p_titleText->SetContent(path.GetNameExt());
        AddWidget(fiw);
    }
}

void Inspector::OnGameObjectSelected(GameObject *selectedGameObject)
{
    SetGameObject(selectedGameObject);
}

void Inspector::OnComponentAdded(Component *addedComponent, int index)
{
    ComponentInspectorWidget *compWidget =
            ComponentInspectorWidgetFactory::Create(addedComponent);
    if (compWidget)
    {
        m_objToWidget.Add(addedComponent, compWidget);
        AddWidget(compWidget, index);
    }
}

void Inspector::OnComponentRemoved(Component *removedComponent)
{
    ASSERT (m_objToWidget.ContainsKey(removedComponent));
    RemoveWidget(m_objToWidget.Get(removedComponent));
    m_objToWidget.Remove(removedComponent);
}

Inspector* Inspector::GetActive()
{
    EditorSceneManager *esm = EditorSceneManager::GetActive();
    EditorScene *es = esm ? esm->GetEditorScene() : nullptr;
    return es ? es->GetInspector() : nullptr;
}

GameObject *Inspector::GetWidgetsContainer() const
{
    return GetScrollPanel()->GetScrollArea()->GetContainedGameObject();
}
UIVerticalLayout *Inspector::GetMainVL() const { return p_mainVL; }
UIScrollPanel* Inspector::GetScrollPanel() const { return p_scrollPanel; }

void Inspector::SetGameObject(GameObject *go)
{
    Clear();
    if(!go || go->IsWaitingToBeDestroyed()) { return; }

    p_currentGameObject = go;
    GetCurrentGameObject()->EventEmitter<IComponentListener>::RegisterListener(this);

    p_titleSeparator->SetEnabled(true);
    p_titleText->SetContent(go->GetName());
    GetCurrentGameObject()->EventEmitter<IDestroyListener>::RegisterListener(this);

    int i = 0;
    for (Component *comp : go->GetComponents())
    {
        OnComponentAdded(comp, i);
        ++i;
    }
}

void Inspector::AddWidget(InspectorWidget *widget, int _index)
{
    int index = _index >= 0 ? _index : m_widgets.Size();

    m_widgets.Insert(index, widget);
    Color bgColor = Color::LightGray.WithValue(0.9f);
    widget->SetBackgroundColor(bgColor);
    widget->SetParent( GetWidgetsContainer(), index );
}

void Inspector::RemoveWidget(InspectorWidget *widget)
{
    m_widgets.Remove(widget);
    widget->SetParent(nullptr);
    GameObject::Destroy(widget);
}

void Inspector::RemoveWidget(int index)
{
    auto it = m_widgets.Begin();
    std::advance(it, index);
    RemoveWidget( *it );
}

void Inspector::Clear()
{
    p_titleText->SetContent("");
    p_titleSeparator->SetEnabled(false);

    while (!m_widgets.IsEmpty())
    {
        InspectorWidget *widget = m_widgets.Front();
        RemoveWidget(widget);
    }

    if (GetCurrentGameObject())
    {
        GetCurrentGameObject()->EventEmitter<IDestroyListener>::UnRegisterListener(this);
        GetCurrentGameObject()->EventEmitter<IComponentListener>::UnRegisterListener(this);
        p_currentGameObject = nullptr;
    }
}
