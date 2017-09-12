#include "Bang/Application.h"

#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/PointLight.h"
#include "Bang/MeshFactory.h"
#include "Bang/SceneManager.h"
#include "Bang/UIGameObject.h"
#include "Bang/MeshRenderer.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/DirectionalLight.h"
#include "Bang/GameObjectFactory.h"

using namespace Bang;

class Rotator : public Component
{
public:
    bool rot = true;

    Rotator() {}

    void OnUpdate() override
    {
        Component::OnUpdate();
        if (Input::GetKeyDown(Input::Key::Space)) { rot = !rot; }
        if (rot)
        {
            GameObject *camGo = SceneManager::GetActiveScene()->GetCamera()->gameObject;
            GameObject *camGoPivot = camGo->parent;
            camGoPivot->transform->Rotate( Quaternion::AngleAxis(0.03f, Vector3::Up) );
            //gameObject->transform->Rotate(Quaternion::AngleAxis(0.014f, Vector3(1)));
        }
    }
};

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
    text->SetTextSize(150);
    textGo->GetComponent<RectTransform>()->SetMarginTop(20);
    text->SetHorizontalAlign(HorizontalAlignment::Center);
    text->SetVerticalAlign(VerticalAlignment::Top);

    textGo->SetParent(scene);

    SceneManager::LoadScene(scene);

    GameObject *plGo = GameObjectFactory::CreateGameObject();
    plGo->transform->SetPosition( Vector3(5) );
    PointLight *light = plGo->AddComponent<PointLight>();
    // DirectionalLight *light = plGo->AddComponent<DirectionalLight>();
    plGo->transform->LookInDirection(Vector3(5,0,0));
    light->SetRange(20.0f);
    light->SetIntensity(1.0f);
    light->SetColor(Color::White);
    plGo->SetParent(scene);

    GameObject *camGoPivot = GameObjectFactory::CreateGameObject();
    GameObject *camGo = GameObjectFactory::CreateGameObject();
    camGo->transform->SetPosition( Vector3(4.4) );

    Camera *cam = camGo->AddComponent<Camera>();
    cam->SetClearColor(Color::DarkBlue);
    scene->SetCamera(cam);
    camGo->SetParent(camGoPivot);
    camGoPivot->SetParent(scene);

    GameObject *sphere = GameObjectFactory::CreateGameObject();
    sphere->AddComponent<Rotator>();
    sphere->transform->SetPosition( Vector3(0) );
    MeshRenderer *spMR = sphere->AddComponent<MeshRenderer>();
    spMR->SetMesh( MeshFactory::GetSphere() );
    spMR->UseMaterialCopy();
    spMR->GetMaterial()->SetDiffuseColor(Color::Green);
    sphere->SetParent(scene);

    camGo->transform->LookAt(sphere->transform->GetPosition());

    return app.MainLoop();
}
