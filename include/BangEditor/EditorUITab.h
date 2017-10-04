#ifndef EDITORUITAB_H
#define EDITORUITAB_H

#include "Bang/GameObject.h"
#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UITextRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class EditorUITab : public GameObject
{
public:
    EditorUITab(const String &title);
    virtual ~EditorUITab();

    UITextRenderer *GetTitleText() const;

private:
    UITextRenderer *p_titleText = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORUITAB_H
