#include "BangEditor/FIWMaterial.h"

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

    p_diffuseColorInput = GameObject::Create<UIInputColor>();
    p_diffuseColorInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_receivesLightingCheckBox = GameObjectFactory::CreateUICheckBox();
    p_receivesLightingCheckBox->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_shininessSlider = GameObjectFactory::CreateUISlider();
    p_shininessSlider->SetMinMaxValues(0.0f, 60.0f);
    p_shininessSlider->EventEmitter<IValueChangedListener>::RegisterListener(this);

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

    AddWidget("Diff Color",   p_diffuseColorInput);
    AddWidget("Rec. light",   p_receivesLightingCheckBox->GetGameObject());
    AddWidget("Shininess",    p_shininessSlider->GetGameObject());
    AddWidget("Texture",      p_texturePathInput);
    AddWidget("Uv Multiply",  p_uvMultiplyInput);
    AddWidget("Render pass",  p_renderPassInput->GetGameObject());
    AddWidget("Vert shader",  p_vertexShaderInput);
    AddWidget("Frag shader",  p_fragmentShaderInput);

    SetLabelsWidth(78);
}

void FIWMaterial::UpdateFromFileWhenChanged()
{
    p_material = Resources::Load<Material>( GetPath() );
    GetMaterial()->ImportXMLFromFile( GetMaterial()->GetResourceFilepath() );

    IValueChangedListener::SetReceiveEvents(false);

    Texture2D *matTex = GetMaterial()->GetTexture();
    p_texturePathInput->SetPath( matTex ? matTex->GetResourceFilepath() : Path::Empty);

    p_uvMultiplyInput->Set( GetMaterial()->GetUvMultiply() );
    p_diffuseColorInput->SetColor( GetMaterial()->GetDiffuseColor() );
    p_receivesLightingCheckBox->SetChecked( GetMaterial()->GetReceivesLighting() );
    p_shininessSlider->SetValue( GetMaterial()->GetShininess() );
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
    return p_material.Get();
}

void FIWMaterial::OnValueChanged(Object *object)
{
    Path texPath = p_texturePathInput->GetPath();
    if (texPath.IsFile())
    {
        RH<Texture2D> tex = Resources::Load<Texture2D>(texPath);
        GetMaterial()->SetTexture(tex.Get());
    }
    else { GetMaterial()->SetTexture(nullptr); }

    GetMaterial()->SetUvMultiply( p_uvMultiplyInput->GetVector2() );
    GetMaterial()->SetDiffuseColor(p_diffuseColorInput->GetColor());
    GetMaterial()->SetReceivesLighting(p_receivesLightingCheckBox->IsChecked());
    GetMaterial()->SetShininess(p_shininessSlider->GetValue());
    GetMaterial()->SetRenderPass(
                SCAST<RenderPass>(p_renderPassInput->GetSelectedValue()) );

    Path vsPath = p_vertexShaderInput->GetPath();
    Path fsPath = p_fragmentShaderInput->GetPath();
    GetMaterial()->SetShaderProgram( ShaderProgramFactory::Get(vsPath,
                                                               fsPath) );

    Path matPath = GetMaterial()->GetResourceFilepath();
    if (matPath.IsFile())
    {
        GetMaterial()->ExportXMLToFile(matPath);
    }
}
