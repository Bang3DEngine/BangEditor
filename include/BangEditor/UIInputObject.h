#ifndef UIINPUTOBJECT_H
#define UIINPUTOBJECT_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Object;
FORWARD class UIInputText;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UIInputObject : public GameObject
{
    GAMEOBJECT_EDITOR(UIInputObject);

public:
    void SetObject(Object *object);

    Object* GetObject() const;

protected:
    UIInputObject();
    virtual ~UIInputObject();

private:
    UIInputText *p_objectInputText = nullptr;
    Object *p_object = nullptr;

};

template <class T>
class UIInputObjectT : public UIInputObject
{

};

NAMESPACE_BANG_EDITOR_END

#endif // UIINPUTOBJECT_H

