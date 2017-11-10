#include "BangEditor/IconManager.h"

#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

IconManager::IconManager()
{
}

IconManager::~IconManager()
{
}

Texture2D *IconManager::GetIcon(const Path &path)
{
    if (path.IsDir()) { return GetIconTexture("Folder"); }
    else
    {
        if (Extensions::Has(path, Extensions::GetImageExtensions()))
        {
            return Resources::Load<Texture2D>(path);
        }
    }
    return GetIcon(path.GetExtension());
}

Texture2D *IconManager::GetIcon(const String &ext)
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

    return GetIconTexture("File");
}

Texture2D *IconManager::GetBackArrowIcon() { return GetIconTexture("BackArrow"); }

Texture2D *IconManager::GetIconTexture(const String &filename)
{
    Texture2D *iconTex =
            Resources::Load<Texture2D>(Paths::EditorResources().
                                       Append("Icons").
                                       Append(filename).
                                       AppendExtension("png"));
    iconTex->SetFilterMode(GL::FilterMode::Linear);
    iconTex->SetWrapMode(GL::WrapMode::ClampToEdge);

    return iconTex;
}

