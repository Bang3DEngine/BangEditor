#include "BangEditor/EditorTextureFactory.h"

#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/TextureFactory.h"

#include "BangEditor/EditorPaths.h"
#include "BangEditor/MaterialPreviewFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

RH<Texture2D> EditorTextureFactory::GetIconForPath(const Path &path)
{
    if (!path.IsDir())
    {
        if ( Extensions::Has(path, Extensions::GetImageExtensions()) )
        {
            Resources::SetPermanent(path, true);
            RH<Texture2D> tex = Resources::Load<Texture2D>(path);
            return tex;
        }
        else if ( Extensions::Has(path, {Extensions::GetMaterialExtension()}) )
        {
            RH<Material> material = Resources::Load<Material>(path);
            return MaterialPreviewFactory::GetPreviewTextureFor(material.Get());
        }
        else
        {
            return EditorTextureFactory::GetIconForExtension(path.GetExtension());
        }
    }
    return EditorTextureFactory::GetFolderIcon();
}

RH<Texture2D> EditorTextureFactory::GetIconForExtension(const String &ext)
{
    if ( Extensions::Equals(ext, Extensions::GetTTFExtensions()) )
    {
        return EditorTextureFactory::GetLetterIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetAudioClipExtensions()) )
    {
        return TextureFactory::GetAudioIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetSceneExtension()) )
    {
        return EditorTextureFactory::GetSceneIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetPrefabExtension()) )
    {
        return EditorTextureFactory::GetPillIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetModelExtensions()) )
    {
        return EditorTextureFactory::GetCubeIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetBehaviourExtensions()) )
    {
        return EditorTextureFactory::GetBracketsIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetTextureCubeMapExtension()) )
    {
        return EditorTextureFactory::GetCubeMapIcon();
    }

    return EditorTextureFactory::GetFileIcon();
}

RH<Texture2D> EditorTextureFactory::GetRotateIcon()
{ return EditorTextureFactory::GetTexture("Rotate"); }
RH<Texture2D> EditorTextureFactory::GetRightArrowAndBarIcon()
{ return EditorTextureFactory::GetTexture("RightArrowAndBar"); }
RH<Texture2D> EditorTextureFactory::GetDoubleBarIcon()
{ return EditorTextureFactory::GetTexture("DoubleBar"); }
RH<Texture2D> EditorTextureFactory::GetBackArrowIcon()
{ return EditorTextureFactory::GetTexture("BackArrow"); }
RH<Texture2D> EditorTextureFactory::GetLensIcon()
{ return EditorTextureFactory::GetTexture("Lens"); }
RH<Texture2D> EditorTextureFactory::GetLensLittleIcon()
{ return EditorTextureFactory::GetTexture("LensLittle"); }
RH<Texture2D> EditorTextureFactory::GetSquareIcon()
{ return EditorTextureFactory::GetTexture("Square"); }
RH<Texture2D> EditorTextureFactory::GetAnchorIcon()
{ return EditorTextureFactory::GetTexture("Anchor"); }
RH<Texture2D> EditorTextureFactory::GetCubeMapIcon()
{ return EditorTextureFactory::GetTexture("CubeMap"); }
RH<Texture2D> EditorTextureFactory::GetWhiteSphereIcon()
{ return EditorTextureFactory::GetTexture("WhiteSphere"); }
RH<Texture2D> EditorTextureFactory::GetFolderIcon()
{ return EditorTextureFactory::GetTexture("Folder"); }
RH<Texture2D> EditorTextureFactory::GetLetterIcon()
{ return EditorTextureFactory::GetTexture("Letter"); }
RH<Texture2D> EditorTextureFactory::GetPillIcon()
{ return EditorTextureFactory::GetTexture("Pill"); }
RH<Texture2D> EditorTextureFactory::GetCubeIcon()
{ return EditorTextureFactory::GetTexture("Cube"); }
RH<Texture2D> EditorTextureFactory::GetBracketsIcon()
{ return EditorTextureFactory::GetTexture("Brackets"); }
RH<Texture2D> EditorTextureFactory::GetFileIcon()
{ return EditorTextureFactory::GetTexture("File"); }
RH<Texture2D> EditorTextureFactory::GetSceneIcon()
{ return EditorTextureFactory::GetTexture("Scene"); }
RH<Texture2D> EditorTextureFactory::GetAxesIcon()
{ return EditorTextureFactory::GetTexture("Axes"); }
RH<Texture2D> EditorTextureFactory::GetHairCrossIcon()
{ return EditorTextureFactory::GetTexture("HairCross"); }
RH<Texture2D> EditorTextureFactory::GetCircleIcon()
{ return EditorTextureFactory::GetTexture("Circle"); }
RH<Texture2D> EditorTextureFactory::GetCircleHardIcon()
{ return EditorTextureFactory::GetTexture("CircleHard"); }
RH<Texture2D> EditorTextureFactory::GetEyeIcon()
{ return EditorTextureFactory::GetTexture("Eye"); }
RH<Texture2D> EditorTextureFactory::GetAnchoredRectIcon()
{ return EditorTextureFactory::GetTexture("AnchoredRect"); }

RH<Texture2D> EditorTextureFactory::GetComponentIcon(const String &componentName)
{
    if (componentName == "Transform") { return EditorTextureFactory::GetAxesIcon(); }
    if (componentName == "RectTransform") { return EditorTextureFactory::GetAnchoredRectIcon(); }
    if (componentName == "PointLight") { return TextureFactory::GetLightBulbIcon(); }
    if (componentName == "DirectionalLight") { return TextureFactory::GetSunIcon(); }
    if (componentName == "Behaviour") { return EditorTextureFactory::GetBracketsIcon(); }
    if (componentName == "MeshRenderer") { return EditorTextureFactory::GetCubeIcon(); }
    if (componentName == "AudioSource") { return TextureFactory::GetAudioIcon(); }
    if (componentName == "AudioListener") { return TextureFactory::GetAudioIcon(); }
    return EditorTextureFactory::GetCubeIcon();
}

RH<Texture2D> EditorTextureFactory::GetTexture(const String &filename)
{
    return TextureFactory::GetTexture(filename,
                                       EditorPaths::GetEditorAssetsDir().
                                       Append("Icons"));
}

