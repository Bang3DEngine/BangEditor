#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "Bang/UIGameObject.h"
#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UITextRenderer;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class Inspector : public UIGameObject
{
public:
    Inspector();
    virtual ~Inspector();

    void Update() override;

private:
    UITextRenderer *m_text = nullptr;
    UIImageRenderer *m_background = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // INSPECTOR_H
