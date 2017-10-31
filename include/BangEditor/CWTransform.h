#ifndef CWTRANSFORM_H
#define CWTRANSFORM_H

#include "BangEditor/InspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Transform;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class CWTransform : public InspectorWidget
{
public:
    CWTransform(Transform *transform);
	virtual ~CWTransform();

private:

};

NAMESPACE_BANG_EDITOR_END

#endif // CWTRANSFORM_H

