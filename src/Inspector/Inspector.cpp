#include "BangEditor/Inspector.h"

#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/XMLNode.h"
#include "Bang/Material.h"
#include "Bang/Selection.h"
#include "Bang/SceneManager.h"
#include "Bang/UIBorderRect.h"
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

    GameObject *mainVLGo = GameObjectFactory::CreateUIGameObject("MainVL");
    mainVLGo->GetComponent<RectTransform>()->SetPivotPosition( Vector2(-1, 1) );

    UIVerticalLayout *mainVL = mainVLGo->AddComponent<UIVerticalLayout>();
    mainVL->SetSpacing(5);

    UIContentSizeFitter *vlCSF = mainVLGo->AddComponent<UIContentSizeFitter>();
    vlCSF->SetVerticalSizeType(LayoutSizeType::Preferred);

    p_mainVL = mainVL;
    p_scrollPanel = scrollPanel;

    GetScrollPanel()->GetScrollArea()->SetContainedGameObject(
                                                GetMainVL()->GetGameObject() );

    SetAsChild(scrollPanel->GetGameObject());
}

Inspector::~Inspector()
{

}

#include "Bang/GL.h"
#include "Bang/GLUniforms.h"
#include "BangEditor/CWTransform.h"
void Inspector::Update()
{
    EditorUITab::Update();

    Scene *openScene = SceneManager::GetActiveScene();
    if (Input::GetMouseButtonDown(MouseButton::Left))
    {
        Rect ndcRect = EditorScene::GetInstance()->GetOpenSceneRectNDC();
        if ( ndcRect.Contains( Input::GetMouseCoordsNDC() ) )
        {
            GameObject *selectedGameObject = Selection::GetOveredGameObject(openScene);
            if (selectedGameObject)
            {
                SetGameObject(selectedGameObject);
            }
            else { SetGameObject(nullptr); }
        }
    }
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

    CWTransform *cwTransform = ObjectManager::Create<CWTransform>(
                                                           go->GetTransform());
    Debug_Log(go->GetTransform());
    AddWidget(cwTransform);

    /*
    for (Component *comp : go)
    {

    }*/
}

void Inspector::AddWidget(InspectorWidget *widget)
{
    m_widgets.PushBack(widget);
    GetContainer()->SetAsChild(widget);
}

void Inspector::RemoveWidget(InspectorWidget *widget)
{
    m_widgets.Remove(widget);
    ObjectManager::Destroy(widget);
}

void Inspector::Clear()
{
    while (!m_widgets.IsEmpty())
    {
        InspectorWidget *widget = m_widgets.Front();
        RemoveWidget(widget);
    }
}
