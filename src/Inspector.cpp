#include "BangEditor/Inspector.h"

#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/XMLNode.h"
#include "Bang/Material.h"
#include "Bang/Selection.h"
#include "Bang/SceneManager.h"
#include "Bang/UIBorderRect.h"
#include "Bang/RectTransform.h"
#include "Bang/UIFrameLayout.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Inspector::Inspector() : EditorUITab("Inspector")
{
    UILayoutElement *le = GetComponent<UILayoutElement>();
    le->SetMinSize( Vector2i(100) );
    le->SetPreferredSize( Vector2i(200) );

    GameObject *propertiesContainer = GameObjectFactory::CreateUIGameObject();
    UIFrameLayout *fl = propertiesContainer->AddComponent<UIFrameLayout>();
    fl->SetChildrenVerticalAlignment(VerticalAlignment::Top);
    fl->SetChildrenHorizontalAlignment(HorizontalAlignment::Left);
    fl->SetPaddings(5);
    AddChild(propertiesContainer);

    GameObject *textCont = GameObjectFactory::CreateUIGameObject();
    p_text = textCont->AddComponent<UITextRenderer>();
    p_text->SetHorizontalAlign(HorizontalAlignment::Left);
    p_text->SetVerticalAlign(VerticalAlignment::Center);
    p_text->SetWrapping(true);
    p_text->SetTextSize(10);
    p_text->SetContent("");
    UILayoutElement *textLE = textCont->AddComponent<UILayoutElement>();

    propertiesContainer->AddChild(textCont);
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
        GameObject *selectedGameObject = Selection::GetOveredGameObject(openScene);
        if (selectedGameObject)
        {
            p_text->SetContent( selectedGameObject->GetXMLInfo().ToString() );
        }
        else { p_text->SetContent(""); }
    }
}
