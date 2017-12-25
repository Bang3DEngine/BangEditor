#include "BangEditor/EditorDialog.h"

#include "Bang/Scene.h"
#include "Bang/Dialog.h"
#include "Bang/UILabel.h"
#include "Bang/GameObject.h"
#include "Bang/SceneManager.h"
#include "Bang/UITextRenderer.h"
#include "Bang/GameObjectFactory.h"


USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

bool EditorDialog::s_okPressed = false;
Path EditorDialog::s_assetPathResult = Path::Empty;

EditorDialog::EditorDialog()
{
}

EditorDialog::~EditorDialog()
{
}

Path EditorDialog::GetAsset(const String &title, const Array<String> &extensions)
{
    DialogWindow *dialog = Dialog::BeginCreateDialog(title, 500, 400, true);

    Scene *scene = GameObjectFactory::CreateScene(false);
    EditorDialog::CreateGetAssetSceneInto(scene, extensions);
    SceneManager::LoadSceneInstantly(scene);

    Dialog::EndCreateDialog(dialog);
    return EditorDialog::s_okPressed ? EditorDialog::s_assetPathResult : Path::Empty;
}


Scene *EditorDialog::CreateGetAssetSceneInto(Scene *scene, const Array<String> &extensions)
{
    GameObjectFactory::CreateUISceneInto(scene);

    UILabel *label = GameObjectFactory::CreateUILabel();
    label->GetText()->SetContent("Hola");

    label->GetGameObject()->SetParent(scene);

    return scene;
}


