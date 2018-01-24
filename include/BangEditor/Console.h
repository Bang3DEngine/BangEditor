#ifndef CONSOLE_H
#define CONSOLE_H

#include "Bang/GameObject.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class Console : public GameObject
{
    GAMEOBJECT_EDITOR(Console);

private:
    Console();
    virtual ~Console();
};

NAMESPACE_BANG_EDITOR_END


#endif // CONSOLE_H
