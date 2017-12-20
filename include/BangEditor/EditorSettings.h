#ifndef EDITORSETTINGS_H
#define EDITORSETTINGS_H

#include "Bang/Bang.h"
#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class EditorSettings
{
public:

private:
    EditorSettings();
	virtual ~EditorSettings();

    static EditorSettings* GetInstance();

    friend class Editor;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORSETTINGS_H

