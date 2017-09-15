#ifndef MENUBARITEM_H
#define MENUBARITEM_H

#include "BangEditor/BangEditor.h"

#include "Bang/UIGameObject.h"
#include "Bang/UITextRenderer.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class MenuBarItem : public UIGameObject
{
public:
    MenuBarItem();
    virtual ~MenuBarItem();

    UITextRenderer *GetText() const;

private:
    UITextRenderer *m_text = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // MENUBARITEM_H
