#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "BangEditor/EditorUITab.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UITextRenderer;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class Inspector : public EditorUITab
{
public:
    Inspector();
    virtual ~Inspector();

    void Update() override;

private:
    UITextRenderer *p_text = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // INSPECTOR_H
