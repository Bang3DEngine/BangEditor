#ifndef EDITORICONMANAGER_H
#define EDITORICONMANAGER_H

#include "Bang/Path.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Texture2D;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN


class EditorIconManager
{
public:
    static Texture2D *GetIcon(const Path &path);
    static Texture2D *GetIcon(const String &extension);
    static Texture2D *GetBackArrowIcon();

private:
    EditorIconManager() = default;
    virtual ~EditorIconManager() = default;

    static Texture2D *GetIconTexture(const String &filename);
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORICONMANAGER_H

