#include "BangEditor/EditorComponentFactory.h"

#include "BangEditor/BehaviourContainer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

#define CREATE_COMPONENT(className, ComponentClass) \
    if (className == ComponentClass::GetClassNameStatic()) \
    { return ComponentFactory::Create<ComponentClass>(); }

#define EXISTS_COMPONENT(componentClassName, ComponentClass) \
    if (componentClassName == ComponentClass::GetClassNameStatic()) \
    { return true; }

Component *EditorComponentFactory::_Create(const String &componentClassName)
{
    CREATE_COMPONENT(componentClassName, BehaviourContainer);

    return ComponentFactory::_Create(componentClassName);
}

bool EditorComponentFactory::_Exists(const String &componentClassName)
{
    EXISTS_COMPONENT(componentClassName, BehaviourContainer);

    return ComponentFactory::_Exists(componentClassName);
}
