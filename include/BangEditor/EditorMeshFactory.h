#ifndef EDITORMESHFACTORY_H
#define EDITORMESHFACTORY_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/MeshFactory.h"
#include "BangEditor/BangEditor.h"

using namespace Bang;
namespace BangEditor
{
class EditorMeshFactory : public MeshFactory
{
private:
    EditorMeshFactory();
    virtual ~EditorMeshFactory() override;

    friend class EditorResources;
};
}

#endif  // EDITORMESHFACTORY_H
