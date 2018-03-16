#ifndef EDITORMESHFACTORY_H
#define EDITORMESHFACTORY_H

#include "Bang/Bang.h"
#include "Bang/MeshFactory.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class EditorMeshFactory : public MeshFactory
{
private:
    EditorMeshFactory();
    virtual ~EditorMeshFactory();

    friend class EditorResources;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORMESHFACTORY_H

