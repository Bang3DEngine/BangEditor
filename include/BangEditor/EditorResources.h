#ifndef EDITORRESOURCES_H
#define EDITORRESOURCES_H

#include "Bang/Resources.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class EditorResources : public Resources
{
public:
	EditorResources();
	virtual ~EditorResources();

private:
    Array<Path> GetLookUpPaths() const override;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORRESOURCES_H

