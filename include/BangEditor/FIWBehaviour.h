#ifndef FIWBEHAVIOUR_H
#define FIWBEHAVIOUR_H

#include "Bang/Bang.h"
#include "BangEditor/BangEditor.h"

#include "BangEditor/FileInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIButton;
FORWARD class UITextRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class FIWBehaviour : public FileInspectorWidget
{
    GAMEOBJECT_EDITOR(FIWBehaviour);

public:
    // InspectorWidget
    void Init() override;

private:
	FIWBehaviour();
	virtual ~FIWBehaviour();

    // FileInspectorWidget
    void UpdateFromFileWhenChanged() override;

    UITextRenderer *p_codeText = nullptr;
    UIButton *p_openButton = nullptr;

    friend class FileInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // FIWBEHAVIOUR_H

