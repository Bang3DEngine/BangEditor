#ifndef EDITORCOMPONENTFACTORY_H
#define EDITORCOMPONENTFACTORY_H

#include "Bang/ComponentFactory.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Component;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class EditorComponentFactory : public ComponentFactory
{
private:
    virtual Component* _Create(const String &componentClassName) override;
    virtual bool _Exists(const String &componentClassName) override;

    EditorComponentFactory() = default;
    virtual ~EditorComponentFactory() = default;

    friend class EditorApplication;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORCOMPONENTFACTORY_H

