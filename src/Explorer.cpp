#include "BangEditor/Explorer.h"

#include "Bang/UILayoutElement.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Explorer::Explorer() : EditorUITab("Explorer")
{
    UILayoutElement *le = GetComponent<UILayoutElement>();
    le->SetMinSize( Vector2i(100, 100) );
    le->SetPreferredWidth(250);
    le->SetFlexibleWidth(9999);
}

Explorer::~Explorer()
{

}
