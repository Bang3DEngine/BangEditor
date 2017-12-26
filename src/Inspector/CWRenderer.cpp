#include "BangEditor/CWRenderer.h"

#include "Bang/Renderer.h"
#include "Bang/UICheckBox.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

CWRenderer::CWRenderer()
{
    SetName("CWRenderer");
    SetTitle("Renderer");

    p_visibleCheckBox = GameObjectFactory::CreateUICheckBox();
    p_visibleCheckBox->GetGameObject()->SetParent( GetContainer() );
    p_visibleCheckBox->EventEmitter<IValueChangedListener>::RegisterListener(this);
}

CWRenderer::~CWRenderer()
{
}

void CWRenderer::Update()
{
    ComponentWidget::Update();

    p_visibleCheckBox->SetChecked( GetRenderer()->IsVisible() );
}

Renderer *CWRenderer::GetRenderer() const
{
    return p_relatedRenderer;
}

void CWRenderer::OnValueChanged(Object *object)
{
    if (object == p_visibleCheckBox)
    {
        GetRenderer()->SetVisible(p_visibleCheckBox->IsChecked());
    }
}

void CWRenderer::SetComponent(Component *comp)
{
    p_relatedRenderer = Cast<Renderer*>(comp);
    ASSERT(p_relatedRenderer);
}

