#include "BangEditor/EditorComponentFactory.h"

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
    return ComponentFactory::_Create(componentClassName);
}

bool EditorComponentFactory::_Exists(const String &componentClassName)
{
    return ComponentFactory::_Exists(componentClassName);
}
