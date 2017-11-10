#ifndef HIERARCHY_H
#define HIERARCHY_H

#include "BangEditor/EditorUITab.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class Hierarchy : public EditorUITab
{
    GAMEOBJECT_EDITOR(Hierarchy)

public:
    Hierarchy();
    virtual ~Hierarchy();
};

NAMESPACE_BANG_EDITOR_END

#endif // HIERARCHY_H
