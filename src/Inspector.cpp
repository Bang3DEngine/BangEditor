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

#include "BangEditor/EditorScene.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Inspector::Inspector() : EditorUITab("Inspector")
{
    UILayoutElement *le = GetComponent<UILayoutElement>();
    le->SetMinSize( Vector2i(100) );
    le->SetPreferredSize( Vector2i(200) );
    le->SetFlexibleSize( Vector2(1) );

    UIScrollPanel *scrollPanel = GameObjectFactory::CreateUIScrollPanel();
    scrollPanel->GetGameObject()->SetName("scrollPanel");
    UILayoutElement *scrollLE = scrollPanel->gameObject->
                                AddComponent<UILayoutElement>();
    scrollPanel->GetScrollArea()->GetBackground()->SetVisible(false);
    scrollLE->SetMinSize( Vector2i::Zero );
    scrollLE->SetFlexibleSize( Vector2(1) );

    GameObject *textCont = GameObjectFactory::CreateUIGameObject();
    textCont->SetName("textCont");
    p_text = textCont->AddComponent<UITextRenderer>();
    p_text->SetHorizontalAlign(HorizontalAlignment::Left);
    p_text->SetVerticalAlign(VerticalAlignment::Center);
    p_text->SetTextColor(Color::Black);
    p_text->SetLayoutMode(UITextRenderer::LayoutMode::MultiLineMinPreferred);
    p_text->SetWrapping(true);
    p_text->SetTextSize(10);
    p_text->SetContent("");

    UILayoutElement *textLE = textCont->AddComponent<UILayoutElement>();
    textLE->SetFlexibleSize( Vector2(1) );

    AddChild(scrollPanel->gameObject);
    scrollPanel->GetContainer()->AddChild(textCont);
}

Inspector::~Inspector()
{

}

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
                p_text->SetContent( selectedGameObject->GetXMLInfo().ToString() );
            }
            else { p_text->SetContent(""); }
        }
    }
}
