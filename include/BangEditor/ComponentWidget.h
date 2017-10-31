#ifndef COMPONENTWIDGET_H
#define COMPONENTWIDGET_H

#include "Bang/Bang.h"

#include "BangEditor/InspectorWidget.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class ComponentWidget : public InspectorWidget
{
public:
	ComponentWidget();
	virtual ~ComponentWidget();
};

NAMESPACE_BANG_EDITOR_END

#endif // COMPONENTWIDGET_H

