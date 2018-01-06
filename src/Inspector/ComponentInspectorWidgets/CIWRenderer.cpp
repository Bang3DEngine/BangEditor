#include "BangEditor/CIWRenderer.h"

#include "Bang/UILabel.h"
#include "Bang/Material.h"
#include "Bang/Renderer.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/UICheckBox.h"
#include "Bang/UITextRenderer.h"

#include "BangEditor/UIInputFile.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

CIWRenderer::CIWRenderer()
{

}

CIWRenderer::~CIWRenderer()
{
}

void CIWRenderer::Init()
{
    ComponentInspectorWidget::Init();

    SetName("CIWRenderer");
    SetTitle("Renderer");

    p_visibleCheckBox = GameObjectFactory::CreateUICheckBox();
    p_visibleCheckBox->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Visible", p_visibleCheckBox->GetGameObject());

    p_materialInputFile = GameObject::Create<UIInputFile>();
    p_materialInputFile->SetExtensions({Extensions::GetMaterialExtension()});
    p_materialInputFile->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Material", p_materialInputFile);

    SetLabelsWidth(60);
}

void CIWRenderer::UpdateValuesFromComponent()
{
    ComponentInspectorWidget::UpdateValuesFromComponent();

    IValueChangedListener::SetReceiveEvents(false);

    p_visibleCheckBox->SetChecked( GetRenderer()->IsVisible() );

    Material *mat = GetRenderer()->GetSharedMaterial();
    Path matPath = mat ? mat->GetResourceFilepath() : Path::Empty;
    p_materialInputFile->SetPath(matPath);

    IValueChangedListener::SetReceiveEvents(true);
}

Renderer *CIWRenderer::GetRenderer() const
{
    return SCAST<Renderer*>( GetComponent() );
}

void CIWRenderer::OnValueChanged(Object *object)
{
    ComponentInspectorWidget::OnValueChanged(object);

    if (object == p_visibleCheckBox)
    {
        GetRenderer()->SetVisible(p_visibleCheckBox->IsChecked());
    }
    else if (object == p_materialInputFile)
    {
        RH<Material> mat = Resources::Load<Material>(p_materialInputFile->GetPath());
        GetRenderer()->SetMaterial(mat.Get());
    }
}
