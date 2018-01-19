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
    static RH<Texture2D> GetIconForPath(const Path &path);
    static RH<Texture2D> GetIconForExtension(const String &extension);

    static RH<Texture2D> GetRightArrowAndBarIcon();
    static RH<Texture2D> GetDoubleBarIcon();
    static RH<Texture2D> GetBackArrowIcon();
    static RH<Texture2D> GetLensIcon();
    static RH<Texture2D> GetLensLittleIcon();
    static RH<Texture2D> GetSquareIcon();
    static RH<Texture2D> GetWhiteSphereIcon();
    static RH<Texture2D> GetFolderIcon();
    static RH<Texture2D> GetLetterIcon();
    static RH<Texture2D> GetAudioIcon();
    static RH<Texture2D> GetPillIcon();
    static RH<Texture2D> GetCubeIcon();
    static RH<Texture2D> GetBracketsIcon();
    static RH<Texture2D> GetFileIcon();
    static RH<Texture2D> GetSceneIcon();
    static RH<Texture2D> GetAxesIcon();
    static RH<Texture2D> GetLightBulbIcon();
    static RH<Texture2D> GetSunIcon();
    static RH<Texture2D> GetHairCrossIcon();
    static RH<Texture2D> GetEyeIcon();
    static RH<Texture2D> GetAnchoredRectIcon();

    static RH<Texture2D> GetComponentIcon(const String &componentName);

private:
    EditorIconManager() = default;
    virtual ~EditorIconManager() = default;

    static RH<Texture2D> GetIconTexture(const String &filename);

};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORICONMANAGER_H

