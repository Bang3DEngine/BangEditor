#include "BangEditor/Hierarchy.h"

#include "Bang/UILayoutElement.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Hierarchy::Hierarchy() : EditorUITab("Hierarchy")
{
    UILayoutElement *le = GetComponent<UILayoutElement>();
    le->SetMinSize( Vector2i(100) );
    le->SetPreferredSize( Vector2i(200) );
}

Hierarchy::~Hierarchy()
{

}
