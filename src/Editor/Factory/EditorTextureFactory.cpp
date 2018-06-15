#include "BangEditor/EditorTextureFactory.h"

#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/TextureFactory.h"

#include "BangEditor/EditorPaths.h"
#include "BangEditor/ModelPreviewFactory.h"
#include "BangEditor/MaterialPreviewFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

RH<Texture2D> EditorTextureFactory::GetIconForPath(const Path &path)
{
    if (!path.IsDir())
    {
        if ( path.HasExtension(Extensions::GetImageExtensions()) )
        {
            Resources::SetPermanent(path, true);
            RH<Texture2D> tex = Resources::Load<Texture2D>(path);
            return tex;
        }
        else if ( path.HasExtension(Extensions::GetModelExtensions()) )
        {
            RH<Model> model = Resources::Load<Model>(path);
            return ModelPreviewFactory::GetPreviewTextureFor(model.Get());
        }
        else if ( path.HasExtension(Extensions::GetMaterialExtension()) )
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

bool EditorTextureFactory::IsIconAnImage(const Path &path)
{
    return !path.HasExtension(Extensions::GetModelExtensions()) &&
           !path.HasExtension(Extensions::GetMaterialExtension());
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
{ return EditorTextureFactory::GetTexture2D("Rotate.png"); }
RH<Texture2D> EditorTextureFactory::GetRightArrowAndBarIcon()
{ return EditorTextureFactory::GetTexture2D("RightArrowAndBar.png"); }
RH<Texture2D> EditorTextureFactory::GetDoubleBarIcon()
{ return EditorTextureFactory::GetTexture2D("DoubleBar.png"); }
RH<Texture2D> EditorTextureFactory::GetBackArrowIcon()
{ return EditorTextureFactory::GetTexture2D("BackArrow.png"); }
RH<Texture2D> EditorTextureFactory::GetLensIcon()
{ return EditorTextureFactory::GetTexture2D("Lens.png"); }
RH<Texture2D> EditorTextureFactory::GetLensLittleIcon()
{ return EditorTextureFactory::GetTexture2D("LensLittle.png"); }
RH<Texture2D> EditorTextureFactory::GetSquareIcon()
{ return EditorTextureFactory::GetTexture2D("Square.png"); }
RH<Texture2D> EditorTextureFactory::GetAnchorIcon()
{ return EditorTextureFactory::GetTexture2D("Anchor.png"); }
RH<Texture2D> EditorTextureFactory::GetCubeMapIcon()
{ return EditorTextureFactory::GetTexture2D("CubeMap.png"); }
RH<Texture2D> EditorTextureFactory::GetWhiteSphereIcon()
{ return EditorTextureFactory::GetTexture2D("WhiteSphere.png"); }
RH<Texture2D> EditorTextureFactory::GetFolderIcon()
{ return EditorTextureFactory::GetTexture2D("Folder.png"); }
RH<Texture2D> EditorTextureFactory::GetLetterIcon()
{ return EditorTextureFactory::GetTexture2D("Letter.png"); }
RH<Texture2D> EditorTextureFactory::GetPillIcon()
{ return EditorTextureFactory::GetTexture2D("Pill.png"); }
RH<Texture2D> EditorTextureFactory::GetCubeIcon()
{ return EditorTextureFactory::GetTexture2D("Cube.png"); }
RH<Texture2D> EditorTextureFactory::GetBracketsIcon()
{ return EditorTextureFactory::GetTexture2D("Brackets.png"); }
RH<Texture2D> EditorTextureFactory::GetFileIcon()
{ return EditorTextureFactory::GetTexture2D("File.png"); }
RH<Texture2D> EditorTextureFactory::GetSceneIcon()
{ return EditorTextureFactory::GetTexture2D("Scene.png"); }
RH<Texture2D> EditorTextureFactory::GetAxesIcon()
{ return EditorTextureFactory::GetTexture2D("Axes.png"); }
RH<Texture2D> EditorTextureFactory::GetHairCrossIcon()
{ return EditorTextureFactory::GetTexture2D("HairCross.png"); }
RH<Texture2D> EditorTextureFactory::GetCircleIcon()
{ return EditorTextureFactory::GetTexture2D("Circle.png"); }
RH<Texture2D> EditorTextureFactory::GetCircleHardIcon()
{ return EditorTextureFactory::GetTexture2D("CircleHard.png"); }
RH<Texture2D> EditorTextureFactory::GetEyeIcon()
{ return EditorTextureFactory::GetTexture2D("Eye.png"); }
RH<Texture2D> EditorTextureFactory::GetAnchoredRectIcon()
{ return EditorTextureFactory::GetTexture2D("AnchoredRect.png"); }

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

RH<Texture2D> EditorTextureFactory::GetTexture2D(const String &filename)
{
    return TextureFactory::GetTexture2D(filename,
                                        EditorPaths::GetEditorAssetsDir().
                                        Append("Textures"));
}

