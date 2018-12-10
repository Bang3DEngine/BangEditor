#ifndef OBJECTITEM_H
#define OBJECTITEM_H

#include "Bang/Bang.h"
#include "Bang/ObjectPtr.h"
#include "BangEditor/NavigatorItem.h"

using namespace Bang;
namespace BangEditor
{
class ObjectItem : public NavigatorItem
{
public:
    ObjectItem();
    virtual ~ObjectItem();

    void SetObject(Object *object);
    Object *GetObject() const;

private:
    ObjectPtr p_objectPtr;
};
}

#endif  // OBJECTITEM_H
