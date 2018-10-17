#ifndef UIINPUTOBJECT_H
#define UIINPUTOBJECT_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class Object;
class UIInputText;
}

using namespace Bang;
namespace BangEditor
{
class UIInputObject : public GameObject
{
    GAMEOBJECT_EDITOR(UIInputObject);

public:
    void SetObject(Object *object);

    Object *GetObject() const;

protected:
    UIInputObject();
    virtual ~UIInputObject() override;

private:
    UIInputText *p_objectInputText = nullptr;
    Object *p_object = nullptr;
};

template <class T>
class UIInputObjectT : public UIInputObject
{
};
}

#endif  // UIINPUTOBJECT_H
