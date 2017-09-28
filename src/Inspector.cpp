#include "BangEditor/Inspector.h"

#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/XMLNode.h"
#include "Bang/Material.h"
#include "Bang/Selection.h"
#include "Bang/SceneManager.h"
#include "Bang/RectTransform.h"
#include "Bang/UIFrameLayout.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Inspector::Inspector()
{
    AddComponent<RectTransform>();
    m_background = AddComponent<UIImageRenderer>();
    m_background->SetTint(Color::LightGray);

    UIFrameLayout *fl = AddComponent<UIFrameLayout>();
    fl->SetChildrenVerticalStretch(Stretch::Full);
    fl->SetChildrenHorizontalStretch(Stretch::Full);
    fl->SetChildrenVerticalAlignment(VerticalAlignment::Top);
    fl->SetChildrenHorizontalAlignment(HorizontalAlignment::Left);
    fl->SetPaddings(5);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetPriority(1);
    le->SetMinWidth(100);
    le->SetPreferredWidth(250);

    UIGameObject *textCont = GameObjectFactory::CreateUIGameObject();

    m_text = textCont->AddComponent<UITextRenderer>();
    m_text->SetWrapping(true);
    textCont->SetParent(this);

    m_text->SetHorizontalAlign(HorizontalAlignment::Left);
    m_text->SetVerticalAlign(VerticalAlignment::Top);
    m_text->SetTextSize(10);

}

Inspector::~Inspector()
{

}

void Inspector::Update()
{
    GameObject::Update();

    Scene *openScene = SceneManager::GetActiveScene();
    if (Input::GetMouseButtonDown(MouseButton::Left))
    {
        GameObject *selectedGameObject = Selection::GetOveredGameObject(openScene);
        if (selectedGameObject)
        {
            m_text->SetContent( selectedGameObject->GetXMLInfo().ToString() );
        }
        else { m_text->SetContent(""); }
        Debug_Log(selectedGameObject);
    }
}
