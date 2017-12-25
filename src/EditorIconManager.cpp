#include "BangEditor/EditorIconManager.h"

#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"

#include "BangEditor/EditorPaths.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

RH<Texture2D> EditorIconManager::GetIcon(const Path &path)
{
    if (!path.IsDir())
    {
        if (Extensions::Has(path, Extensions::GetImageExtensions()))
        {
            return Resources::Load<Texture2D>(path);
        }
        else { return GetIcon(path.GetExtension()); }
    }
    return GetIconTexture("Folder");
}

RH<Texture2D> EditorIconManager::GetIcon(const String &ext)
{
    if (ext == Extensions::GetFontExtension() ||
        Extensions::Equals(ext, Extensions::GetTTFExtensions()) )
    {
        return GetIconTexture("Letter");
    }
    else if (ext == Extensions::GetAudioClipExtension())
    {
        return GetIconTexture("Audio");
    }
    else if (ext == Extensions::GetSceneExtension())
    {
        return GetIconTexture("Scene");
    }
    else if (ext == Extensions::GetPrefabExtension())
    {
        return GetIconTexture("Pill");
    }
    else if (Extensions::Equals(ext, Extensions::GetModelExtensions()))
    {
        return GetIconTexture("Cube");
    }
    else if (Extensions::Equals(ext, Extensions::GetBehaviourExtensions()))
    {
        return GetIconTexture("Brackets");
    }
    else if (Extensions::Equals(ext, Extensions::GetImageExtensions()))
    {
        return GetIconTexture("Image");
    }
    else
    {
        return GetIconTexture("File");
    }
}

RH<Texture2D> EditorIconManager::GetBackArrowIcon()
{ return GetIconTexture("BackArrow"); }
RH<Texture2D> EditorIconManager::GetLensIcon()
{ return GetIconTexture("Lens"); }
RH<Texture2D> EditorIconManager::GetLensLittleIcon()
{ return GetIconTexture("LensLittle"); }

RH<Texture2D> EditorIconManager::GetIconTexture(const String &filename)
{
    RH<Texture2D> iconTex =
        Resources::Load<Texture2D>(EditorPaths::EditorResources().Append("Icons").
                                   Append(filename).AppendExtension("png"));
    iconTex.Get()->SetFilterMode(GL::FilterMode::Bilinear);
    iconTex.Get()->SetWrapMode(GL::WrapMode::ClampToEdge);
    return iconTex;
}

