#ifndef UIINPUTARRAYROW_H
#define UIINPUTARRAYROW_H

#include "Bang/GameObject.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputArray;

class UIInputArrayRow : public GameObject
{
    GAMEOBJECT_EDITOR(UIInputArrayRow);

public:
    UIInputArrayRow();
    virtual ~UIInputArrayRow();

    void Init(UIInputArray *inputArray);
    void SetContainedGameObject(GameObject *containedGo);

    GameObject *GetContainedGameObject() const;

private:
    GameObject *p_containedGameObject = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // UIINPUTARRAYROW_H

