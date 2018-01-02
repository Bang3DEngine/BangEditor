#include "BangEditor/Console.h"

#include "Bang/UILayoutElement.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Console::Console() : EditorUITab("Console")
{
    UILayoutElement *le = GetLayoutElement();
    le->SetMinSize( Vector2i(100, 100) );
    le->SetPreferredWidth(250);
}

Console::~Console()
{

}
