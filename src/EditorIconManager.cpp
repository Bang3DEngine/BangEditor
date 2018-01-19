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
            return Resources::Load<Texture2D>(path);
        }
        else { return GetIconForExtension(path.GetExtension()); }
    }
    return GetFolderIcon();
}

RH<Texture2D> EditorIconManager::GetIconForExtension(const String &ext)
{
    if ( Extensions::Equals(ext, Extensions::GetTTFExtensions()) )
    {
        return GetLetterIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetAudioClipExtensions()) )
    {
        return GetAudioIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetSceneExtension()) )
    {
        return GetSceneIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetPrefabExtension()) )
    {
        return GetPillIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetModelExtensions()) )
    {
        return GetCubeIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetBehaviourExtensions()) )
    {
        return GetBracketsIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetMaterialExtension()) )
    {
        return GetWhiteSphereIcon();
    }
    else
    {
        return GetFileIcon();
    }
}

RH<Texture2D> EditorIconManager::GetRightArrowAndBarIcon()
{ return GetIconTexture("RightArrowAndBar"); }
RH<Texture2D> EditorIconManager::GetDoubleBarIcon()
{ return GetIconTexture("DoubleBar"); }
RH<Texture2D> EditorIconManager::GetBackArrowIcon()
{ return GetIconTexture("BackArrow"); }
RH<Texture2D> EditorIconManager::GetLensIcon()
{ return GetIconTexture("Lens"); }
RH<Texture2D> EditorIconManager::GetLensLittleIcon()
{ return GetIconTexture("LensLittle"); }
RH<Texture2D> EditorIconManager::GetSquareIcon()
{ return GetIconTexture("Square"); }
RH<Texture2D> EditorIconManager::GetWhiteSphereIcon()
{ return GetIconTexture("WhiteSphere"); }
RH<Texture2D> EditorIconManager::GetFolderIcon()
{ return GetIconTexture("Folder"); }
RH<Texture2D> EditorIconManager::GetLetterIcon()
{ return GetIconTexture("Letter"); }
RH<Texture2D> EditorIconManager::GetAudioIcon()
{ return GetIconTexture("Audio"); }
RH<Texture2D> EditorIconManager::GetPillIcon()
{ return GetIconTexture("Pill"); }
RH<Texture2D> EditorIconManager::GetCubeIcon()
{ return GetIconTexture("Cube"); }
RH<Texture2D> EditorIconManager::GetBracketsIcon()
{ return GetIconTexture("Brackets"); }
RH<Texture2D> EditorIconManager::GetFileIcon()
{ return GetIconTexture("File"); }
RH<Texture2D> EditorIconManager::GetSceneIcon()
{ return GetIconTexture("Scene"); }
RH<Texture2D> EditorIconManager::GetAxesIcon()
{ return GetIconTexture("Axes"); }
RH<Texture2D> EditorIconManager::GetLightBulbIcon()
{ return GetIconTexture("LightBulb"); }
RH<Texture2D> EditorIconManager::GetSunIcon()
{ return GetIconTexture("Sun"); }
RH<Texture2D> EditorIconManager::GetHairCrossIcon()
{ return GetIconTexture("HairCross"); }
RH<Texture2D> EditorIconManager::GetEyeIcon()
{ return GetIconTexture("Eye"); }
RH<Texture2D> EditorIconManager::GetAnchoredRectIcon()
{ return GetIconTexture("AnchoredRect"); }

RH<Texture2D> EditorIconManager::GetComponentIcon(const String &componentName)
{
    if (componentName == "Transform") { return EditorIconManager::GetAxesIcon(); }
    if (componentName == "RectTransform") { return EditorIconManager::GetAnchoredRectIcon(); }
    if (componentName == "PointLight") { return EditorIconManager::GetLightBulbIcon(); }
    if (componentName == "DirectionalLight") { return EditorIconManager::GetSunIcon(); }
    if (componentName == "Behaviour") { return EditorIconManager::GetBracketsIcon(); }
    if (componentName == "MeshRenderer") { return EditorIconManager::GetCubeIcon(); }
    if (componentName == "AudioSource") { return EditorIconManager::GetAudioIcon(); }
    if (componentName == "AudioListener") { return EditorIconManager::GetAudioIcon(); }
    return GetCubeIcon();
}

RH<Texture2D> EditorIconManager::GetIconTexture(const String &filename)
{
    static Map<Path, RH<Texture2D>> cache;

    Path path = EditorPaths::GetEditorResourcesDir().Append("Icons").
                             Append(filename).AppendExtension("png");
    if (!cache.ContainsKey(path))
    {
        RH<Texture2D> iconTex = Resources::Load<Texture2D>(path);
        iconTex.Get()->GenerateMipMaps();
        iconTex.Get()->SetFilterMode(GL::FilterMode::Trilinear_LL);
        iconTex.Get()->SetWrapMode(GL::WrapMode::ClampToEdge);
        cache.Add(path, iconTex);
    }
    return cache.Get(path);
}

