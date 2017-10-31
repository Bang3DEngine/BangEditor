#ifndef INSPECTORWIDGET_H
#define INSPECTORWIDGET_H

#include "Bang/GameObject.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class InspectorWidget : public GameObject
{
public:
	InspectorWidget();
	virtual ~InspectorWidget();
};

NAMESPACE_BANG_EDITOR_END

#endif // INSPECTORWIDGET_H

