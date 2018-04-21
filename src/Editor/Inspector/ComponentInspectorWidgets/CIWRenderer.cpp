#include "BangEditor/CIWRenderer.h"

#include "Bang/UILabel.h"
#include "Bang/Material.h"
#include "Bang/Renderer.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/UICheckBox.h"
#include "Bang/UITextRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UIInputFile.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

CIWRenderer::CIWRenderer()
{

}

CIWRenderer::~CIWRenderer()
{
}

void CIWRenderer::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWRenderer");
    SetTitle("Renderer");

    p_visibleCheckBox = GameObjectFactory::CreateUICheckBox();
    p_visibleCheckBox->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_castsShadowsCheckBox = GameObjectFactory::CreateUICheckBox();
    p_castsShadowsCheckBox->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_receivesShadowsCheckBox = GameObjectFactory::CreateUICheckBox();
    p_receivesShadowsCheckBox->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_materialInputFile = GameObject::Create<UIInputFile>();
    p_materialInputFile->SetExtensions({Extensions::GetMaterialExtension()});
    p_materialInputFile->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Visible", p_visibleCheckBox->GetGameObject());
    AddWidget("Casts Shadows", p_castsShadowsCheckBox->GetGameObject());
    AddWidget("Receives Shadows", p_receivesShadowsCheckBox->GetGameObject());
    AddWidget("Material", p_materialInputFile);

    SetLabelsWidth(60);
}

void CIWRenderer::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    IValueChangedListener::SetReceiveEvents(false);

    p_visibleCheckBox->SetChecked( GetRenderer()->IsVisible() );
    p_castsShadowsCheckBox->SetChecked( GetRenderer()->GetCastsShadows() );
    p_receivesShadowsCheckBox->SetChecked( GetRenderer()->GetReceivesShadows() );

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
    else if (object == p_castsShadowsCheckBox)
    {
        GetRenderer()->SetCastsShadows(p_castsShadowsCheckBox->IsChecked());
    }
    else if (object == p_receivesShadowsCheckBox)
    {
        GetRenderer()->SetReceivesShadows(p_receivesShadowsCheckBox->IsChecked());
    }
    else if (object == p_materialInputFile)
    {
        RH<Material> mat = Resources::Load<Material>(p_materialInputFile->GetPath());
        GetRenderer()->SetMaterial(mat.Get());
    }
}
