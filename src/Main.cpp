#include "Bang/Application.h"

#include "Bang/Scene.h"
#include "Bang/Transform.h"
#include "Bang/SceneManager.h"
#include "Bang/UIGameObject.h"
#include "Bang/UITextRenderer.h"
#include "Bang/GameObjectFactory.h"

using namespace Bang;

int main(int argc, char **argv)
{
    Application app(argc, argv);
    app.CreateWindow();

    Scene *scene = new Scene();
    scene->AddComponent<Transform>();

    UIGameObject *textGo = GameObjectFactory::CreateUIGameObject();
    UITextRenderer *text = textGo->AddComponent<UITextRenderer>();
    text->SetContent("Bang");
    text->SetTextColor(Color::Orange);
    text->SetOutlineColor(Color::Yellow);
    text->SetOutlineWidth(0.05f);
    text->SetOutlineBlurriness(0.01f);
    text->SetTextSize(250);
    text->SetHorizontalAlign(HorizontalAlignment::Center);
    text->SetVerticalAlign(VerticalAlignment::Center);

    textGo->SetParent(scene);

    SceneManager::LoadScene(scene);

    return app.MainLoop();
}
