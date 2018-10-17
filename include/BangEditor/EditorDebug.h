#ifndef EDITORDEBUG_H
#define EDITORDEBUG_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/Debug.h"
#include "BangEditor/BangEditor.h"

using namespace Bang;
namespace BangEditor
{
class EditorDebug : public Debug
{
public:
    EditorDebug();
    virtual ~EditorDebug() override;

    friend class EditorApplication;
};
}

#endif  // EDITORDEBUG_H
