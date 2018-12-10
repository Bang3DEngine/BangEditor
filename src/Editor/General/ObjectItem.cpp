#include "BangEditor/ObjectItem.h"

using namespace Bang;
using namespace BangEditor;

ObjectItem::ObjectItem()
{
}

ObjectItem::~ObjectItem()
{
}

void ObjectItem::SetObject(Object *object)
{
    if (object != p_objectPtr.Get())
    {
        p_objectPtr = object;

        String labelText = "None";
        if (GetObject())
        {
            if (GameObject *go = DCAST<GameObject *>(GetObject()))
            {
                labelText = go->GetName();
            }

            if (Component *comp = DCAST<Component *>(GetObject()))
            {
                labelText = comp->GetGameObject()->GetName() + "/" +
                            comp->GetClassName();
            }
        }

        GetLabel()->GetText()->SetContent(labelText);
    }
}

Object *ObjectItem::GetObject() const
{
    return p_objectPtr.Get();
}
