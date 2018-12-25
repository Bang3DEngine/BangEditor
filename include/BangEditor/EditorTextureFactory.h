#ifndef EDITORTEXTUREFACTORY_H
#define EDITORTEXTUREFACTORY_H

#include <functional>

#include "Bang/AssetHandle.h"
#include "Bang/BangDefines.h"
#include "Bang/Color.h"
#include "Bang/Map.h"
#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/TextureFactory.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class Component;
class Texture2D;
}

using namespace Bang;
namespace BangEditor
{
class EditorTextureFactory : public TextureFactory
{
public:
    static Texture2D *GetAddIcon();
    static Texture2D *GetLessIcon();
    static Texture2D *GetStarsIcon();
    static Texture2D *GetWorldIcon();
    static Texture2D *GetCameraIcon();
    static Texture2D *GetRotateIcon();
    static Texture2D *GetGreenCubeIcon();
    static Texture2D *GetAnimatorSMIcon();
    static Texture2D *GetAnimatorLayerMaskIcon();
    static Texture2D *GetGreenSphereIcon();
    static Texture2D *GetGreenCapsuleIcon();
    static Texture2D *GetCrashDummyIcon();
    static Texture2D *GetPlayWithCircleIcon();
    static Texture2D *GetRightArrowAndBarIcon();
    static Texture2D *GetDoubleBarIcon();
    static Texture2D *GetBackArrowIcon();
    static Texture2D *GetLensIcon();
    static Texture2D *GetLensLittleIcon();
    static Texture2D *GetSquareIcon();
    static Texture2D *GetAnchorIcon();
    static Texture2D *GetCubeMapIcon();
    static Texture2D *GetGrid2x2();
    static Texture2D *GetWhiteSphereIcon();
    static Texture2D *GetFolderIcon();
    static Texture2D *GetLetterIcon();
    static Texture2D *GetImageIcon();
    static Texture2D *GetSplashIcon();
    static Texture2D *GetPillIcon();
    static Texture2D *GetCubeIcon();
    static Texture2D *GetBracketsIcon();
    static Texture2D *GetPhysicsBallIcon();
    static Texture2D *GetFileIcon();
    static Texture2D *GetSceneIcon();
    static Texture2D *GetAxesIcon();
    static Texture2D *GetColoredAxesIcon();
    static Texture2D *GetHairCrossIcon();
    static Texture2D *GetCircleIcon();
    static Texture2D *GetCircleHardIcon();
    static Texture2D *GetEyeIcon();
    static Texture2D *GetAnchoredRectIcon();

    static bool IsIconAnImage(const Path &path);

    static Texture2D *GetIconForPath(const Path &path);
    static Texture2D *GetIconForExtension(const String &extension);
    static Color GetPathIconTint(const Path &path);
    static Color GetExtensionIconTint(const String &extension);

    static Texture2D *GetComponentIcon(const Component *component);
    static Texture2D *GetComponentIcon(const String &componentName);
    static Color GetComponentIconTint(const Component *component);
    static Color GetComponentIconTint(const String &componentName);

private:
    Map<Path, AH<Texture2D>> m_iconCacheMap;

    EditorTextureFactory() = default;
    virtual ~EditorTextureFactory() = default;

    static EditorTextureFactory *GetInstance();
    static Texture2D *GetTexture2D(const String &filename);

    friend class EditorAssets;
};
}

#endif  // EDITORTEXTUREFACTORY_H
