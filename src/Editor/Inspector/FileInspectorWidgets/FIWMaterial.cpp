#include "BangEditor/FIWMaterial.h"

#include "Bang/Model.h"
#include "Bang/Shader.h"
#include "Bang/Material.h"
#include "Bang/UISlider.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"
#include "Bang/RenderPass.h"
#include "Bang/Extensions.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIComboBox.h"
#include "Bang/ShaderProgram.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/ImportFilesManager.h"
#include "Bang/ShaderProgramFactory.h"

#include "BangEditor/UIInputVector.h"
#include "BangEditor/UIInputFile.h"
#include "BangEditor/UIInputColor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

FIWMaterial::FIWMaterial()
{
}

FIWMaterial::~FIWMaterial()
{
}

void FIWMaterial::Init()
{
    FileInspectorWidget::Init();

    SetName("FIWMaterial");
    SetTitle("Material");

    p_texturePathInput = GameObject::Create<UIInputFile>();
    p_texturePathInput->SetExtensions( Extensions::GetImageExtensions() );
    p_texturePathInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_uvMultiplyInput = GameObject::Create<UIInputVector>();
    p_uvMultiplyInput->SetSize(2);
    p_uvMultiplyInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_albedoColorInput = GameObject::Create<UIInputColor>();
    p_albedoColorInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_receivesLightingCheckBox = GameObjectFactory::CreateUICheckBox();
    p_receivesLightingCheckBox->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_roughnessSlider = GameObjectFactory::CreateUISlider();
    p_roughnessSlider->SetMinMaxValues(0.0f, 1.0f);
    p_roughnessSlider->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_metalnessSlider = GameObjectFactory::CreateUISlider();
    p_metalnessSlider->SetMinMaxValues(0.0f, 1.0f);
    p_metalnessSlider->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_renderPassInput = GameObjectFactory::CreateUIComboBox();
    p_renderPassInput->AddItem("Scene",              SCAST<int>(RenderPass::Scene) );
    p_renderPassInput->AddItem("Canvas",             SCAST<int>(RenderPass::Canvas) );
    p_renderPassInput->AddItem("Overlay",            SCAST<int>(RenderPass::Overlay) );
    p_renderPassInput->AddItem("ScenePostProcess",   SCAST<int>(RenderPass::ScenePostProcess) );
    p_renderPassInput->AddItem("CanvasPostProcess",  SCAST<int>(RenderPass::CanvasPostProcess) );
    p_renderPassInput->AddItem("OverlayPostProcess", SCAST<int>(RenderPass::OverlayPostProcess) );
    p_renderPassInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_vertexShaderInput = GameObject::Create<UIInputFile>();
    p_vertexShaderInput->SetExtensions( Extensions::GetVertexShaderExtensions() );
    p_vertexShaderInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_fragmentShaderInput = GameObject::Create<UIInputFile>();
    p_fragmentShaderInput->SetExtensions( Extensions::GetFragmentShaderExtensions() );
    p_fragmentShaderInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    AddWidget("Albedo Color", p_albedoColorInput);
    AddWidget("Rec. light",   p_receivesLightingCheckBox->GetGameObject());
    AddWidget("Roughness",    p_roughnessSlider->GetGameObject());
    AddWidget("Metalness",    p_metalnessSlider->GetGameObject());
    AddWidget("Texture",      p_texturePathInput);
    AddWidget("Uv Multiply",  p_uvMultiplyInput);
    AddWidget("Render pass",  p_renderPassInput->GetGameObject());
    AddWidget("Vert shader",  p_vertexShaderInput);
    AddWidget("Frag shader",  p_fragmentShaderInput);

    SetLabelsWidth(78);
}

void FIWMaterial::UpdateFromFileWhenChanged()
{
    if (GetPath().IsFile()) // Typical material in path
    {
        m_materialRH = Resources::Load<Material>( GetPath() );
    }
    else if (GetPath().GetDirectory().IsFile())
    {
        // Material inside other asset but without file (for example, inside a model)
        Path containingAsset = GetPath().GetDirectory();
        if (containingAsset.HasExtension(Extensions::GetModelExtensions()))
        {
            RH<Model> model = Resources::Load<Model>(containingAsset);
            m_materialRH = model.Get()->GetMaterialByName(GetPath().GetName());
        }
    }

    if (!GetMaterial()) { return; }

    GetMaterial()->ImportXMLFromFile( GetMaterial()->GetResourceFilepath() );

    IValueChangedListener::SetReceiveEvents(false);

    Texture2D *matTex = GetMaterial()->GetTexture();
    p_texturePathInput->SetPath( matTex ? matTex->GetResourceFilepath() : Path::Empty);

    p_uvMultiplyInput->Set( GetMaterial()->GetUvMultiply() );
    p_albedoColorInput->SetColor( GetMaterial()->GetAlbedoColor() );
    p_receivesLightingCheckBox->SetChecked( GetMaterial()->GetReceivesLighting() );
    p_roughnessSlider->SetValue( GetMaterial()->GetRoughness() );
    p_metalnessSlider->SetValue( GetMaterial()->GetMetalness() );
    p_renderPassInput->SetSelectionByValue(
                SCAST<int>(GetMaterial()->GetRenderPass()) );

    ShaderProgram *sp = GetMaterial()->GetShaderProgram();
    Shader *vs = sp ? sp->GetVertexShader() : nullptr;
    Shader *fs = sp ? sp->GetFragmentShader() : nullptr;
    p_vertexShaderInput->SetPath  ( vs ? vs->GetResourceFilepath() : Path::Empty );
    p_fragmentShaderInput->SetPath( fs ? fs->GetResourceFilepath() : Path::Empty );

    IValueChangedListener::SetReceiveEvents(true);
}

Material *FIWMaterial::GetMaterial() const
{
    return m_materialRH.Get();
}

void FIWMaterial::OnValueChanged(Object *)
{
    if (!GetMaterial()) { return; }

    Path texPath = p_texturePathInput->GetPath();
    if (texPath.IsFile())
    {
        RH<Texture2D> tex = Resources::Load<Texture2D>(texPath);
        GetMaterial()->SetTexture(tex.Get());
    }
    else { GetMaterial()->SetTexture(nullptr); }

    GetMaterial()->SetUvMultiply( p_uvMultiplyInput->GetVector2() );
    GetMaterial()->SetAlbedoColor(p_albedoColorInput->GetColor());
    GetMaterial()->SetReceivesLighting(p_receivesLightingCheckBox->IsChecked());
    GetMaterial()->SetRoughness(p_roughnessSlider->GetValue());
    GetMaterial()->SetMetalness(p_metalnessSlider->GetValue());
    GetMaterial()->SetRenderPass(
                SCAST<RenderPass>(p_renderPassInput->GetSelectedValue()) );

    Path vsPath = p_vertexShaderInput->GetPath();
    Path fsPath = p_fragmentShaderInput->GetPath();
    GetMaterial()->SetShaderProgram( ShaderProgramFactory::Get(vsPath,
                                                               fsPath) );

    GUID matGUID = GetMaterial()->GetGUID();
    Path importPath = ImportFilesManager::GetImportFilepath(matGUID);
    if (importPath.IsFile())
    {
        GetMaterial()->ExportXMLToFile(importPath);
    }
}
