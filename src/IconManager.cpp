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
    return GetIcon(path.GetExtension());
}

Texture2D *IconManager::GetIcon(const String &extension)
{
    if (extension == Extensions::GetFontExtension())
    {

    }
    else if (extension == Extensions::GetAudioClipExtension())
    {

    }
    else if (extension == Extensions::GetSceneExtension())
    {

    }
    else if (extension == Extensions::GetAudioClipExtension())
    {

    }
    else if (extension == Extensions::GetAudioClipExtension())
    {

    }
    else if (extension == Extensions::GetAudioClipExtension())
    {

    }
    else if (extension == Extensions::GetAudioClipExtension())
    {

    }
    else if (extension == Extensions::GetAudioClipExtension())
    {

    }
    else if (extension == Extensions::GetAudioClipExtension())
    {

    }

    return nullptr;
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

