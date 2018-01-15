#ifndef EDITORICONMANAGER_H
#define EDITORICONMANAGER_H

#include "Bang/Path.h"
#include "Bang/ResourceHandle.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Texture2D;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN


class EditorIconManager
{
public:
    static RH<Texture2D> GetIcon(const Path &path);
    static RH<Texture2D> GetIcon(const String &extension);
    static RH<Texture2D> GetBackArrowIcon();
    static RH<Texture2D> GetLensIcon();
    static RH<Texture2D> GetLensLittleIcon();
    static RH<Texture2D> GetSquareIcon();
    static RH<Texture2D> GetWhiteSphereIcon();

private:
    EditorIconManager() = default;
    virtual ~EditorIconManager() = default;

    static RH<Texture2D> GetIconTexture(const String &filename);
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORICONMANAGER_H

