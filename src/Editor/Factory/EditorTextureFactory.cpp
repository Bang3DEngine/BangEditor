#include "BangEditor/EditorTextureFactory.h"

#include "Bang/Model.h"
#include "Bang/Material.h"
#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/MeshFactory.h"
#include "Bang/TextureFactory.h"

#include "BangEditor/EditorPaths.h"
#include "BangEditor/MeshPreviewFactory.h"
#include "BangEditor/ModelPreviewFactory.h"
#include "BangEditor/MaterialPreviewFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Texture2D* EditorTextureFactory::GetIconForPath(const Path &path)
{
    if (!path.IsDir())
    {
        EditorTextureFactory *etf = EditorTextureFactory::GetInstance();

        if (etf->m_iconCacheMap.ContainsKey(path))
        {
            return etf->m_iconCacheMap.Get(path).Get();
        }

        RH<Texture2D> icon;
        if ( path.HasExtension(Extensions::GetImageExtensions()) )
        {
            icon = Resources::Load<Texture2D>(path);
        }
        else if ( path.HasExtension(Extensions::GetModelExtensions()) )
        {
            RH<Model> model = Resources::Load<Model>(path);
            icon = ModelPreviewFactory::GetPreviewTextureFor(model.Get());
        }
        else if ( path.HasExtension(Extensions::GetMeshExtension()) )
        {
            RH<Mesh> mesh = Resources::Load<Mesh>(path);
            icon = MeshPreviewFactory::GetPreviewTextureFor(mesh.Get());
        }
        else if ( path.HasExtension(Extensions::GetMaterialExtension()) )
        {
            RH<Material> material = Resources::Load<Material>(path);
            icon = MaterialPreviewFactory::GetPreviewTextureFor(material.Get());
        }
        else
        {
            icon.Set( EditorTextureFactory::GetIconForExtension(path.GetExtension()) );
        }

        if (icon)
        {
            etf->m_iconCacheMap.Add(path, icon);
        }

        return icon.Get();
    }
    return EditorTextureFactory::GetFolderIcon();
}

bool EditorTextureFactory::IsIconAnImage(const Path &path)
{
    return !path.HasExtension(Extensions::GetModelExtensions()) &&
           !path.HasExtension(Extensions::GetMaterialExtension());
}

Texture2D* EditorTextureFactory::GetRotateIcon()
{
    return EditorTextureFactory::GetTexture2D("Rotate.png");
}

Texture2D *EditorTextureFactory::GetGreenCubeIcon()
{
    return EditorTextureFactory::GetTexture2D("GreenCube.png");
}

Texture2D *EditorTextureFactory::GetGreenSphereIcon()
{
    return EditorTextureFactory::GetTexture2D("GreenSphere.png");
}

Texture2D *EditorTextureFactory::GetGreenCapsuleIcon()
{
    return EditorTextureFactory::GetTexture2D("GreenCapsule.png");
}

Texture2D *EditorTextureFactory::GetCrashDummyIcon()
{
    return EditorTextureFactory::GetTexture2D("CrashDummy.png");
}

Texture2D *EditorTextureFactory::GetPlayWithCircleIcon()
{
    return EditorTextureFactory::GetTexture2D("PlayWithCircle.png");
}

Texture2D* EditorTextureFactory::GetRightArrowAndBarIcon()
{
    return EditorTextureFactory::GetTexture2D("RightArrowAndBar.png");
}

Texture2D* EditorTextureFactory::GetDoubleBarIcon()
{
    return EditorTextureFactory::GetTexture2D("DoubleBar.png");
}

Texture2D* EditorTextureFactory::GetBackArrowIcon()
{
    return EditorTextureFactory::GetTexture2D("BackArrow.png");
}

Texture2D* EditorTextureFactory::GetLensIcon()
{
    return EditorTextureFactory::GetTexture2D("Lens.png");
}

Texture2D* EditorTextureFactory::GetLensLittleIcon()
{
    return EditorTextureFactory::GetTexture2D("LensLittle.png");
}

Texture2D* EditorTextureFactory::GetSquareIcon()
{
    return EditorTextureFactory::GetTexture2D("Square.png");
}

Texture2D* EditorTextureFactory::GetAnchorIcon()
{
    return EditorTextureFactory::GetTexture2D("Anchor.png");
}

Texture2D* EditorTextureFactory::GetCubeMapIcon()
{
    return EditorTextureFactory::GetTexture2D("CubeMap.png");
}

Texture2D* EditorTextureFactory::GetWhiteSphereIcon()
{
    return EditorTextureFactory::GetTexture2D("WhiteSphere.png");
}

Texture2D* EditorTextureFactory::GetFolderIcon()
{
    return EditorTextureFactory::GetTexture2D("Folder.png");
}

Texture2D* EditorTextureFactory::GetLetterIcon()
{
    return EditorTextureFactory::GetTexture2D("Letter.png");
}

Texture2D* EditorTextureFactory::GetPillIcon()
{
    return EditorTextureFactory::GetTexture2D("Pill.png");
}

Texture2D* EditorTextureFactory::GetCubeIcon()
{
    return EditorTextureFactory::GetTexture2D("Cube.png");
}

Texture2D* EditorTextureFactory::GetBracketsIcon()
{
    return EditorTextureFactory::GetTexture2D("Brackets.png");
}

Texture2D *EditorTextureFactory::GetPhysicsBallIcon()
{
    return EditorTextureFactory::GetTexture2D("PhysicsBall.png");
}

Texture2D* EditorTextureFactory::GetFileIcon()
{
    return EditorTextureFactory::GetTexture2D("File.png");
}

Texture2D* EditorTextureFactory::GetSceneIcon()
{
    return EditorTextureFactory::GetTexture2D("Scene.png");
}

Texture2D* EditorTextureFactory::GetAxesIcon()
{
    return EditorTextureFactory::GetTexture2D("Axes.png");
}

Texture2D *EditorTextureFactory::GetColoredAxesIcon()
{
    return EditorTextureFactory::GetTexture2D("ColoredAxes.png");
}

Texture2D* EditorTextureFactory::GetHairCrossIcon()
{
    return EditorTextureFactory::GetTexture2D("HairCross.png");
}

Texture2D* EditorTextureFactory::GetCircleIcon()
{
    return EditorTextureFactory::GetTexture2D("Circle.png");
}

Texture2D* EditorTextureFactory::GetCircleHardIcon()
{
    return EditorTextureFactory::GetTexture2D("CircleHard.png");
}

Texture2D* EditorTextureFactory::GetEyeIcon()
{
    return EditorTextureFactory::GetTexture2D("Eye.png");
}

Texture2D* EditorTextureFactory::GetAnchoredRectIcon()
{
    return EditorTextureFactory::GetTexture2D("AnchoredRect.png");
}

Texture2D* EditorTextureFactory::GetComponentIcon(const String &componentName)
{
    if (componentName == "Transform")
    {
        return EditorTextureFactory::GetColoredAxesIcon();
    }
    if (componentName == "Behaviour" || componentName == "BehaviourContainer")
    {
        return EditorTextureFactory::GetBracketsIcon();
    }
    if (componentName == "RectTransform")
    {
        return EditorTextureFactory::GetAnchoredRectIcon();
    }
    if (componentName == "BoxCollider")
    {
        return EditorTextureFactory::GetGreenCubeIcon();
    }
    if (componentName == "SphereCollider")
    {
        return EditorTextureFactory::GetGreenSphereIcon();
    }
    if (componentName == "CapsuleCollider")
    {
        return EditorTextureFactory::GetGreenCapsuleIcon();
    }
    if (componentName == "RigidBody")
    {
        return EditorTextureFactory::GetCrashDummyIcon();
    }
    if (componentName == "PointLight")
    {
        return TextureFactory::GetLightBulbIcon();
    }
    if (componentName == "DirectionalLight")
    {
        return TextureFactory::GetSunIcon();
    }
    if (componentName == "Behaviour")
    {
        return EditorTextureFactory::GetBracketsIcon();
    }
    if (componentName == "MeshRenderer")
    {
        return EditorTextureFactory::GetCubeIcon();
    }
    if (componentName == "Animator")
    {
        return EditorTextureFactory::GetPlayWithCircleIcon();
    }
    if (componentName == "AudioSource")
    {
        return TextureFactory::GetAudioIcon();
    }
    if (componentName == "AudioListener")
    {
        return TextureFactory::GetAudioIcon();
    }

    return EditorTextureFactory::GetCubeIcon();
}

Color EditorTextureFactory::GetComponentIconTint(const String &componentName)
{
    if (componentName == "MeshRenderer")
    {
        return Color::White.WithValue(0.2f);
    }
    else if (componentName == "SkinnedMeshRenderer")
    {
        return Color::White.WithValue(0.2f);
    }
    else if (componentName == "DirectionalLight")
    {
        return Color::White.WithValue(0.2f);
    }
    else if (componentName == "PointLight")
    {
        return Color::White.WithValue(0.2f);
    }
    else if (componentName == "BehaviourContainer")
    {
        return Color::White.WithValue(0.2f);
    }
    return Color::White;
}

Texture2D* EditorTextureFactory::GetIconForExtension(const String &ext)
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
    else if ( Extensions::Equals(ext, Extensions::GetPhysicsMaterialExtension()) )
    {
        return EditorTextureFactory::GetPhysicsBallIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetBehaviourExtensions()) )
    {
        return EditorTextureFactory::GetBracketsIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetTextureCubeMapExtension()) )
    {
        return EditorTextureFactory::GetCubeMapIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetAnimationExtension()) )
    {
        return EditorTextureFactory::GetPlayWithCircleIcon();
    }
    else if ( Extensions::Equals(ext, Extensions::GetMeshExtension()) )
    {
        return EditorTextureFactory::GetCubeIcon();
    }

    return EditorTextureFactory::GetFileIcon();
}

Color EditorTextureFactory::GetPathIconTint(const Path &path)
{
    return EditorTextureFactory::GetExtensionIconTint(path.GetExtension());
}

Color EditorTextureFactory::GetExtensionIconTint(const String &extension)
{
    if (Extensions::Equals(extension, Extensions::GetSceneExtension()))
    {
        return Color::Black;
    }
    else if (Extensions::Equals(extension, Extensions::GetBehaviourExtensions()))
    {
        return Color::Black;
    }
    return Color::White;
}

EditorTextureFactory *EditorTextureFactory::GetInstance()
{
    return SCAST<EditorTextureFactory*>( TextureFactory::GetInstance() );
}

Texture2D* EditorTextureFactory::GetTexture2D(const String &filename)
{
    return TextureFactory::GetTexture2D(filename,
                                        EditorPaths::GetEditorAssetsDir().
                                        Append("Textures"));
}

