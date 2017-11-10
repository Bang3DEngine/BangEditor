#ifndef CONSOLE_H
#define CONSOLE_H

#include "BangEditor/EditorUITab.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class Console : public EditorUITab
{
    GAMEOBJECT_EDITOR(Console)

public:
    Console();
    virtual ~Console();
};

NAMESPACE_BANG_EDITOR_END


#endif // CONSOLE_H
