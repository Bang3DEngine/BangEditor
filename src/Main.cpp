#include "Bang/Application.h"

#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/PointLight.h"
#include "Bang/MeshFactory.h"
#include "Bang/SceneManager.h"
#include "Bang/UIGameObject.h"
#include "Bang/MeshRenderer.h"
#include "Bang/UITextRenderer.h"
#include "Bang/GameObjectFactory.h"

using namespace Bang;

class Rotator : public Component
{
public:
    Rotator() {}

    void OnUpdate() override
    {
        Component::OnUpdate();
        GameObject *camGo = SceneManager::GetActiveScene()->GetCamera()->gameObject;
        // camGo->transform->Rotate( Quaternion::AngleAxis(0.03f, camGo->transform->GetForward()) );
        gameObject->transform->Rotate(Quaternion::AngleAxis(0.014f, Vector3(1)));
        //gameObject->transform->Translate(Vector3::Forward * -0.01f);
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
    text->SetTextSize(250);
    text->SetHorizontalAlign(HorizontalAlignment::Center);
    text->SetVerticalAlign(VerticalAlignment::Center);
    text->SetVisible(false);

    textGo->SetParent(scene);

    SceneManager::LoadScene(scene);

    GameObject *plGo = GameObjectFactory::CreateGameObject();
    plGo->transform->SetPosition( Vector3(5) );
    PointLight *pl = plGo->AddComponent<PointLight>();
    pl->SetRange(30.0f);
    pl->SetIntensity(1.0f);
    pl->SetColor(Color::White);
    plGo->SetParent(scene);

    GameObject *camGo = GameObjectFactory::CreateGameObject();
    camGo->transform->SetPosition( Vector3(2, 2, 0) );

    Camera *cam = camGo->AddComponent<Camera>();
    cam->SetClearColor(Color::White);
    scene->SetCamera(cam);
    camGo->SetParent(scene);

    GameObject *sphere = GameObjectFactory::CreateGameObject();
    sphere->AddComponent<Rotator>();
    sphere->transform->SetPosition( Vector3(0) );
    MeshRenderer *spMR = sphere->AddComponent<MeshRenderer>();
    spMR->SetMesh( MeshFactory::GetSphere() );
    spMR->UseMaterialCopy();
    spMR->GetMaterial()->SetDiffuseColor(Color::Red);
    sphere->SetParent(scene);

    camGo->transform->LookAt(sphere->transform->GetPosition());

    return app.MainLoop();
}
