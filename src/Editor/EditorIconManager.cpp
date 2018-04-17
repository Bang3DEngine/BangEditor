#include "BangEditor/EditorIconManager.h"

#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/IconManager.h"

#include "BangEditor/EditorPaths.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

RH<Texture2D> EditorIconManager::GetIconForPath(const Path &path)
{
    if (!path.IsDir())
    {
        if ( Extensions::Has(path, Extensions::GetImageExtensions()) )
        {
            Resources::SetPermanent(path, true);
            RH<Texture2D> tex = Resources::Load<Texture2D>(path);
            return tex;
        }
        else
        {
            return EditorIconManager::GetIconForExtension(path.GetExtension());
        }
    }
    return EditorIconManager::GetFolderIcon();
}

RH<Texture2D> EditorIconManager::GetIconForExtension(const String &ext)
{
    if ( Extensions::Equals(ext, Extensions::GetTTFExtensions()) )
    {
        return EditorIconManager::GetLetterIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetAudioClipExtensions()) )
    {
        return IconManager::GetAudioIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetSceneExtension()) )
    {
        return EditorIconManager::GetSceneIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetPrefabExtension()) )
    {
        return EditorIconManager::GetPillIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetModelExtensions()) )
    {
        return EditorIconManager::GetCubeIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetBehaviourExtensions()) )
    {
        return EditorIconManager::GetBracketsIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetMaterialExtension()) )
    {
        return EditorIconManager::GetWhiteSphereIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetTextureCubeMapExtension()) )
    {
        return EditorIconManager::GetCubeMapIcon();
    }

    return EditorIconManager::GetFileIcon();
}

RH<Texture2D> EditorIconManager::GetRightArrowAndBarIcon()
{ return EditorIconManager::GetIconTexture("RightArrowAndBar"); }
RH<Texture2D> EditorIconManager::GetDoubleBarIcon()
{ return EditorIconManager::GetIconTexture("DoubleBar"); }
RH<Texture2D> EditorIconManager::GetBackArrowIcon()
{ return EditorIconManager::GetIconTexture("BackArrow"); }
RH<Texture2D> EditorIconManager::GetLensIcon()
{ return EditorIconManager::GetIconTexture("Lens"); }
RH<Texture2D> EditorIconManager::GetLensLittleIcon()
{ return EditorIconManager::GetIconTexture("LensLittle"); }
RH<Texture2D> EditorIconManager::GetSquareIcon()
{ return EditorIconManager::GetIconTexture("Square"); }
RH<Texture2D> EditorIconManager::GetAnchorIcon()
{ return EditorIconManager::GetIconTexture("Anchor"); }
RH<Texture2D> EditorIconManager::GetCubeMapIcon()
{ return EditorIconManager::GetIconTexture("CubeMap"); }
RH<Texture2D> EditorIconManager::GetWhiteSphereIcon()
{ return EditorIconManager::GetIconTexture("WhiteSphere"); }
RH<Texture2D> EditorIconManager::GetFolderIcon()
{ return EditorIconManager::GetIconTexture("Folder"); }
RH<Texture2D> EditorIconManager::GetLetterIcon()
{ return EditorIconManager::GetIconTexture("Letter"); }
RH<Texture2D> EditorIconManager::GetPillIcon()
{ return EditorIconManager::GetIconTexture("Pill"); }
RH<Texture2D> EditorIconManager::GetCubeIcon()
{ return EditorIconManager::GetIconTexture("Cube"); }
RH<Texture2D> EditorIconManager::GetBracketsIcon()
{ return EditorIconManager::GetIconTexture("Brackets"); }
RH<Texture2D> EditorIconManager::GetFileIcon()
{ return EditorIconManager::GetIconTexture("File"); }
RH<Texture2D> EditorIconManager::GetSceneIcon()
{ return EditorIconManager::GetIconTexture("Scene"); }
RH<Texture2D> EditorIconManager::GetAxesIcon()
{ return EditorIconManager::GetIconTexture("Axes"); }
RH<Texture2D> EditorIconManager::GetHairCrossIcon()
{ return EditorIconManager::GetIconTexture("HairCross"); }
RH<Texture2D> EditorIconManager::GetCircleIcon()
{ return EditorIconManager::GetIconTexture("Circle"); }
RH<Texture2D> EditorIconManager::GetCircleHardIcon()
{ return EditorIconManager::GetIconTexture("CircleHard"); }
RH<Texture2D> EditorIconManager::GetEyeIcon()
{ return EditorIconManager::GetIconTexture("Eye"); }
RH<Texture2D> EditorIconManager::GetAnchoredRectIcon()
{ return EditorIconManager::GetIconTexture("AnchoredRect"); }

RH<Texture2D> EditorIconManager::GetComponentIcon(const String &componentName)
{
    if (componentName == "Transform") { return EditorIconManager::GetAxesIcon(); }
    if (componentName == "RectTransform") { return EditorIconManager::GetAnchoredRectIcon(); }
    if (componentName == "PointLight") { return IconManager::GetLightBulbIcon(); }
    if (componentName == "DirectionalLight") { return IconManager::GetSunIcon(); }
    if (componentName == "Behaviour") { return EditorIconManager::GetBracketsIcon(); }
    if (componentName == "MeshRenderer") { return EditorIconManager::GetCubeIcon(); }
    if (componentName == "AudioSource") { return IconManager::GetAudioIcon(); }
    if (componentName == "AudioListener") { return IconManager::GetAudioIcon(); }
    return EditorIconManager::GetCubeIcon();
}

RH<Texture2D> EditorIconManager::GetIconTexture(const String &filename)
{
    return IconManager::GetIconTexture(filename,
                                       EditorPaths::GetEditorAssetsDir().
                                       Append("Icons"));
}

