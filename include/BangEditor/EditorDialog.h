#ifndef EDITORDIALOG_H
#define EDITORDIALOG_H

#include "Bang/Array.h"
#include "Bang/String.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class EditorDialog
{
public:
	EditorDialog();
	virtual ~EditorDialog();

    static void GetAsset(const String &title,
                         const Array<String> &extensions,
                         Path *resultPath,
                         bool *accepted);

private:
    static bool s_accepted;
    static Path s_assetPathResult;

    static Scene* CreateGetAssetSceneInto(Scene *scene,
                                     const Array<String> &extensions);
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORDIALOG_H

