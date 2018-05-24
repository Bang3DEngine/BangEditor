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
#include "Bang/RectTransform.h"
#include "Bang/TextureFactory.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/ImportFilesManager.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIContentSizeFitter.h"
#include "Bang/UIAspectRatioFitter.h"
#include "Bang/ShaderProgramFactory.h"

#include "BangEditor/UIInputFile.h"
#include "BangEditor/UIInputColor.h"
#include "BangEditor/UIInputVector.h"
#include "BangEditor/MaterialPreviewFactory.h"

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

    p_albedoTextureInput = GameObject::Create<UIInputFile>();
    p_albedoTextureInput->SetExtensions( Extensions::GetImageExtensions() );
    p_albedoTextureInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_albedoUvMultiplyInput = GameObject::Create<UIInputVector>();
    p_albedoUvMultiplyInput->SetSize(2);
    p_albedoUvMultiplyInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_albedoUvOffsetInput = GameObject::Create<UIInputVector>();
    p_albedoUvOffsetInput->SetSize(2);
    p_albedoUvOffsetInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_normalMapTextureInput = GameObject::Create<UIInputFile>();
    p_normalMapTextureInput->SetExtensions( Extensions::GetImageExtensions() );
    p_normalMapTextureInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_normalMapUvMultiplyInput = GameObject::Create<UIInputVector>();
    p_normalMapUvMultiplyInput->SetSize(2);
    p_normalMapUvMultiplyInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_normalMapUvOffsetInput = GameObject::Create<UIInputVector>();
    p_normalMapUvOffsetInput->SetSize(2);
    p_normalMapUvOffsetInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_normalMapMultiplyFactorInput = GameObjectFactory::CreateUIInputNumber();
    p_normalMapMultiplyFactorInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

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
    p_renderPassInput->AddItem("Scene",              SCAST<int>(RenderPass::SCENE) );
    p_renderPassInput->AddItem("Canvas",             SCAST<int>(RenderPass::CANVAS) );
    p_renderPassInput->AddItem("Overlay",            SCAST<int>(RenderPass::OVERLAY) );
    p_renderPassInput->AddItem("ScenePostProcess",   SCAST<int>(RenderPass::SCENE_POSTPROCESS) );
    p_renderPassInput->AddItem("CanvasPostProcess",  SCAST<int>(RenderPass::CANVAS_POSTPROCESS) );
    p_renderPassInput->AddItem("OverlayPostProcess", SCAST<int>(RenderPass::OVERLAY_POSTPROCESS) );
    p_renderPassInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_vertexShaderInput = GameObject::Create<UIInputFile>();
    p_vertexShaderInput->SetExtensions( Extensions::GetVertexShaderExtensions() );
    p_vertexShaderInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_fragmentShaderInput = GameObject::Create<UIInputFile>();
    p_fragmentShaderInput->SetExtensions( Extensions::GetFragmentShaderExtensions() );
    p_fragmentShaderInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_lineWidthInput = GameObjectFactory::CreateUIInputNumber();
    p_lineWidthInput->SetMinMaxValues(1.0f, Math::Infinity<float>());
    p_lineWidthInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_cullFaceInput = GameObjectFactory::CreateUIComboBox();
    p_cullFaceInput->AddItem("None",         SCAST<int>(GL::CullFaceExt::NONE) );
    p_cullFaceInput->AddItem("Back",         SCAST<int>(GL::CullFaceExt::BACK) );
    p_cullFaceInput->AddItem("Front",        SCAST<int>(GL::CullFaceExt::FRONT) );
    p_cullFaceInput->AddItem("FrontAndBack", SCAST<int>(GL::CullFaceExt::FRONT_AND_BACK) );
    p_cullFaceInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_renderWireframe = GameObjectFactory::CreateUICheckBox();
    p_renderWireframe->EventEmitter<IValueChangedListener>::RegisterListener(this);

    GameObject *materialPreviewGo = GameObjectFactory::CreateUIGameObject();
    materialPreviewGo->GetRectTransform()->SetAnchors(Vector2::Zero);
    materialPreviewGo->GetRectTransform()->SetPivotPosition(Vector2::Zero);

    // UILayoutElement *materialPreviewGoLE = materialPreviewGo->
    //                                        AddComponent<UILayoutElement>();
    // materialPreviewGoLE->SetFlexibleSize( Vector2::One );

    p_materialPreviewImg = materialPreviewGo->AddComponent<UIImageRenderer>();
    p_materialPreviewImg->SetImageTexture( TextureFactory::GetWhiteTexture().Get() );

    UIContentSizeFitter *previewContentSizeFitter =
                     materialPreviewGo->AddComponent<UIContentSizeFitter>();
    previewContentSizeFitter->SetVerticalSizeType(LayoutSizeType::PREFERRED);
    previewContentSizeFitter->SetHorizontalSizeType(LayoutSizeType::PREFERRED);

    UIAspectRatioFitter *previewAspectRatioSizeFitter =
                     materialPreviewGo->AddComponent<UIAspectRatioFitter>();
    previewAspectRatioSizeFitter->SetAspectRatio(1.0f);
    previewAspectRatioSizeFitter->SetAspectRatioMode(AspectRatioMode::KEEP);

    AddWidget(materialPreviewGo,      256);
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Rec. light",           p_receivesLightingCheckBox->GetGameObject());
    AddWidget("Roughness",            p_roughnessSlider->GetGameObject());
    AddWidget("Metalness",            p_metalnessSlider->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Albedo Color",         p_albedoColorInput);
    AddWidget("Albedo Tex.",          p_albedoTextureInput);
    AddWidget("Albedo Uv Offset",     p_albedoUvOffsetInput);
    AddWidget("Albedo Uv Mult.",      p_albedoUvMultiplyInput);
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Normal Map Tex.",      p_normalMapTextureInput);
    AddWidget("Normal Map Uv Offset", p_normalMapUvOffsetInput);
    AddWidget("Normal Map Uv Mult.",  p_normalMapUvMultiplyInput);
    AddWidget("Normal Map Factor",    p_normalMapMultiplyFactorInput->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Render pass",          p_renderPassInput->GetGameObject());
    AddWidget("Cull Face",            p_cullFaceInput->GetGameObject());
    AddWidget("Render Wireframe",     p_renderWireframe->GetGameObject());
    AddWidget("Line Width",           p_lineWidthInput->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Vert shader",          p_vertexShaderInput);
    AddWidget("Frag shader",          p_fragmentShaderInput);

    SetLabelsWidth(130);
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

    Texture2D *albedoTex = GetMaterial()->GetAlbedoTexture();
    p_albedoTextureInput->SetPath( albedoTex ? albedoTex->GetResourceFilepath() :
                                               Path::Empty);
    p_albedoUvOffsetInput->Set( GetMaterial()->GetAlbedoUvOffset() );
    p_albedoUvMultiplyInput->Set( GetMaterial()->GetAlbedoUvMultiply() );

    Texture2D *normalMapTex = GetMaterial()->GetNormalMapTexture();
    p_normalMapTextureInput->SetPath( normalMapTex ?
                                      normalMapTex->GetResourceFilepath() :
                                      Path::Empty);
    p_normalMapUvOffsetInput->Set( GetMaterial()->GetNormalMapUvOffset() );
    p_normalMapUvMultiplyInput->Set( GetMaterial()->GetNormalMapUvMultiply() );
    p_normalMapMultiplyFactorInput->SetValue( GetMaterial()->GetNormalMapMultiplyFactor() );

    p_albedoColorInput->SetColor( GetMaterial()->GetAlbedoColor() );
    p_receivesLightingCheckBox->SetChecked( GetMaterial()->GetReceivesLighting() );
    p_roughnessSlider->SetValue( GetMaterial()->GetRoughness() );
    p_metalnessSlider->SetValue( GetMaterial()->GetMetalness() );
    p_renderPassInput->SetSelectionByValue(
                SCAST<int>(GetMaterial()->GetRenderPass()) );

    p_renderWireframe->SetChecked( GetMaterial()->IsRenderWireframe() );
    p_cullFaceInput->SetSelectionByValue( SCAST<int>(GetMaterial()->GetCullFace()) );
    p_lineWidthInput->SetValue( GetMaterial()->GetLineWidth() );

    p_materialPreviewImg->SetImageTexture(
            MaterialPreviewFactory::GetPreviewTextureFor(GetMaterial()).Get() );

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

    Path albedoTexPath = p_albedoTextureInput->GetPath();
    if (albedoTexPath.IsFile())
    {
        RH<Texture2D> tex = Resources::Load<Texture2D>(albedoTexPath);
        GetMaterial()->SetAlbedoTexture(tex.Get());
    }
    else { GetMaterial()->SetAlbedoTexture(nullptr); }

    Path normalMapTexPath = p_normalMapTextureInput->GetPath();
    if (normalMapTexPath.IsFile())
    {
        RH<Texture2D> tex = Resources::Load<Texture2D>(normalMapTexPath);
        GetMaterial()->SetNormalMapTexture(tex.Get());
    }
    else { GetMaterial()->SetNormalMapTexture(nullptr); }

    GetMaterial()->SetAlbedoColor(p_albedoColorInput->GetColor());
    GetMaterial()->SetAlbedoUvOffset( p_albedoUvOffsetInput->GetVector2() );
    GetMaterial()->SetAlbedoUvMultiply( p_albedoUvMultiplyInput->GetVector2() );
    GetMaterial()->SetNormalMapUvOffset( p_normalMapUvOffsetInput->GetVector2() );
    GetMaterial()->SetNormalMapUvMultiply( p_normalMapUvMultiplyInput->GetVector2() );
    GetMaterial()->SetNormalMapMultiplyFactor( p_normalMapMultiplyFactorInput->GetValue() );
    GetMaterial()->SetReceivesLighting(p_receivesLightingCheckBox->IsChecked());
    GetMaterial()->SetRoughness(p_roughnessSlider->GetValue());
    GetMaterial()->SetMetalness(p_metalnessSlider->GetValue());
    GetMaterial()->SetRenderPass(
                SCAST<RenderPass>(p_renderPassInput->GetSelectedValue()) );
    GetMaterial()->SetRenderWireframe( p_renderWireframe->IsChecked() );
    GetMaterial()->SetCullFace(
                SCAST<GL::CullFaceExt>(p_cullFaceInput->GetSelectedValue()) );
    GetMaterial()->SetLineWidth( p_lineWidthInput->GetValue() );

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
