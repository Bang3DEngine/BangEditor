#include "BangEditor/Hierarchy.h"

#include "Bang/UILayoutElement.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Hierarchy::Hierarchy() : EditorUITab("Hierarchy")
{
    UILayoutElement *le = GetComponent<UILayoutElement>();
    le->SetMinWidth(100);
    le->SetPreferredWidth(250);
}

Hierarchy::~Hierarchy()
{

}
