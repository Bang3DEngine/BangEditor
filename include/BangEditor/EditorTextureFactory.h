#ifndef EDITORTEXTUREFACTORY_H
#define EDITORTEXTUREFACTORY_H

#include "Bang/Path.h"
#include "Bang/ResourceHandle.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Texture2D;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN


class EditorTextureFactory
{
public:
    static RH<Texture2D> GetIconForPath(const Path &path);
    static RH<Texture2D> GetIconForExtension(const String &extension);

    static RH<Texture2D> GetRotateIcon();
    static RH<Texture2D> GetRightArrowAndBarIcon();
    static RH<Texture2D> GetDoubleBarIcon();
    static RH<Texture2D> GetBackArrowIcon();
    static RH<Texture2D> GetLensIcon();
    static RH<Texture2D> GetLensLittleIcon();
    static RH<Texture2D> GetSquareIcon();
    static RH<Texture2D> GetAnchorIcon();
    static RH<Texture2D> GetCubeMapIcon();
    static RH<Texture2D> GetWhiteSphereIcon();
    static RH<Texture2D> GetFolderIcon();
    static RH<Texture2D> GetLetterIcon();
    static RH<Texture2D> GetPillIcon();
    static RH<Texture2D> GetCubeIcon();
    static RH<Texture2D> GetBracketsIcon();
    static RH<Texture2D> GetFileIcon();
    static RH<Texture2D> GetSceneIcon();
    static RH<Texture2D> GetAxesIcon();
    static RH<Texture2D> GetHairCrossIcon();
    static RH<Texture2D> GetCircleIcon();
    static RH<Texture2D> GetCircleHardIcon();
    static RH<Texture2D> GetEyeIcon();
    static RH<Texture2D> GetAnchoredRectIcon();

    static RH<Texture2D> GetComponentIcon(const String &componentName);

private:
    EditorTextureFactory() = default;
    virtual ~EditorTextureFactory() = default;

    static RH<Texture2D> GetTexture2D(const String &filename);

};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORTEXTUREFACTORY_H

