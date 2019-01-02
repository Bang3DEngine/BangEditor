#ifndef EDITORPROJECT_H
#define EDITORPROJECT_H

#include "Bang/Bang.h"
#include "Bang/Project.h"
#include "BangEditor/BangEditor.h"

using namespace Bang;
namespace BangEditor
{
class EditorProject : public Project
{
public:
    EditorProject();
    virtual ~EditorProject();

    bool OpenInitialScene() const override;
};
}

#endif  // EDITORPROJECT_H
