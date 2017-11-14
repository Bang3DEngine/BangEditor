#ifndef HIERARCHYITEM_H
#define HIERARCHYITEM_H

#include "Bang/UIList.h"
#include "Bang/GameObject.h"
#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UILabel;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class HierarchyItem : public GameObject
{
    GAMEOBJECT_EDITOR(HierarchyItem);

public:
    HierarchyItem();
    virtual ~HierarchyItem();


    void SetReferencedGameObject(GameObject *referencedGameObject);
    GameObject *GetReferencedGameObject() const;

    void OnSelectionCallback(UIList::Action action);

private:
    String m_text = "";
    GameObject *p_refGameObject = nullptr;

    UILabel *p_label = nullptr;

    void SetText(const String &text);
};

NAMESPACE_BANG_EDITOR_END

#endif // HIERARCHYITEM_H

