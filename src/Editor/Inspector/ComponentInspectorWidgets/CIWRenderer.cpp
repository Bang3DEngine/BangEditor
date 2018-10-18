#include "BangEditor/CIWRenderer.h"

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/EventListener.tcc"
#include "Bang/Extensions.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEvents.h"
#include "Bang/Material.h"
#include "Bang/Path.h"
#include "Bang/Renderer.h"
#include "Bang/ResourceHandle.h"
#include "Bang/Resources.h"
#include "Bang/Resources.tcc"
#include "Bang/String.h"
#include "Bang/UICheckBox.h"
#include "BangEditor/UIInputFileWithPreview.h"

namespace Bang
{
class IEventsValueChanged;
}

using namespace Bang;
using namespace BangEditor;

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
    p_visibleCheckBox->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_castsShadowsCheckBox = GameObjectFactory::CreateUICheckBox();
    p_castsShadowsCheckBox->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_receivesShadowsCheckBox = GameObjectFactory::CreateUICheckBox();
    p_receivesShadowsCheckBox
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_useReflectionProbesCheckBox = GameObjectFactory::CreateUICheckBox();
    p_useReflectionProbesCheckBox
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_materialInputFile = new UIInputFileWithPreview();
    p_materialInputFile->SetExtensions({Extensions::GetMaterialExtension()});
    p_materialInputFile->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    AddWidget("Visible", p_visibleCheckBox->GetGameObject());
    AddWidget("Casts Shadows", p_castsShadowsCheckBox->GetGameObject());
    AddWidget("Receives Shadows", p_receivesShadowsCheckBox->GetGameObject());
    AddWidget("Use Reflection Probes",
              p_useReflectionProbesCheckBox->GetGameObject());
    AddWidget("Material", p_materialInputFile);

    SetLabelsWidth(60);
}

void CIWRenderer::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    EventListener<IEventsValueChanged>::SetReceiveEvents(false);

    p_visibleCheckBox->SetChecked(GetRenderer()->IsVisible());
    p_castsShadowsCheckBox->SetChecked(GetRenderer()->GetCastsShadows());
    p_receivesShadowsCheckBox->SetChecked(GetRenderer()->GetReceivesShadows());
    p_useReflectionProbesCheckBox->SetChecked(
        GetRenderer()->GetUseReflectionProbes());

    Material *mat = GetRenderer()->GetSharedMaterial();
    Path matPath = mat ? mat->GetResourceFilepath() : Path::Empty;
    p_materialInputFile->SetPath(matPath);

    EventListener<IEventsValueChanged>::SetReceiveEvents(true);
}

Renderer *CIWRenderer::GetRenderer() const
{
    return SCAST<Renderer *>(GetComponent());
}

void CIWRenderer::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

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
        GetRenderer()->SetReceivesShadows(
            p_receivesShadowsCheckBox->IsChecked());
    }
    else if (object == p_useReflectionProbesCheckBox)
    {
        GetRenderer()->SetUseReflectionProbes(
            p_useReflectionProbesCheckBox->IsChecked());
    }
    else if (object == p_materialInputFile)
    {
        RH<Material> mat =
            Resources::Load<Material>(p_materialInputFile->GetPath());
        GetRenderer()->SetMaterial(mat.Get());
    }
}
