#include "BangEditor/Console.h"

#include "Bang/UILayoutElement.h"
#include "Bang/UIRendererCacher.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Console::Console()
{
    SetName("Console");

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleSize( Vector2::One );

    GameObjectFactory::CreateUIGameObjectInto(this);
    UIRendererCacher *rendCacher = GameObjectFactory::CreateUIRendererCacherInto(this);
    GameObject *rendererCacherContainer = rendCacher->GetContainer();
    (void)(rendererCacherContainer);
}

Console::~Console()
{

}
