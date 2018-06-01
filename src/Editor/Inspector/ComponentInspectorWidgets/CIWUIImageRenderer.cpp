#include "BangEditor/CIWUIImageRenderer.h"

#include "Bang/UICanvas.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/GameObject.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UIInputFile.h"
#include "BangEditor/UIInputColor.h"
#include "BangEditor/UIInputTexture.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

CIWUIImageRenderer::CIWUIImageRenderer()
{
}

CIWUIImageRenderer::~CIWUIImageRenderer()
{
}

void CIWUIImageRenderer::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWUIImageRenderer");
    SetTitle("UI Image Renderer");

    p_tintInput = GameObject::Create<UIInputColor>();

    p_imageInput = GameObject::Create<UIInputTexture>();
    p_imageInput->SetExtensions( Extensions::GetImageExtensions() );

    p_tintInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_imageInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Image", p_imageInput);
    AddWidget("Tint",  p_tintInput);

    SetLabelsWidth(70);
}

void CIWUIImageRenderer::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    if (!p_tintInput->HasFocus())
    {
        p_tintInput->SetColor( GetUIImageRenderer()->GetTint() );
    }

    Texture2D *tex = GetUIImageRenderer()->GetImageTexture();
    p_imageInput->SetPath(tex ? tex->GetResourceFilepath() : Path::Empty);
}

UIImageRenderer *CIWUIImageRenderer::GetUIImageRenderer() const
{
    return SCAST<UIImageRenderer*>( GetComponent() );
}

void CIWUIImageRenderer::OnValueChanged(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChanged(object);

    GetUIImageRenderer()->SetTint( p_tintInput->GetColor() );
    GetUIImageRenderer()->SetImageTexture( p_imageInput->GetPath() );
}

