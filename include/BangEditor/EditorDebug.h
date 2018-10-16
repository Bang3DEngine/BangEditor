#ifndef EDITORDEBUG_H
#define EDITORDEBUG_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/Debug.h"
#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class EditorDebug : public Debug
{
public:
	EditorDebug();
	virtual ~EditorDebug() override;

    friend class EditorApplication;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORDEBUG_H

