#include "BangEditor/CIWUIImageRenderer.h"

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/Extensions.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/IEvents.h"
#include "Bang/Path.h"
#include "Bang/Texture2D.h"
#include "Bang/UIImageRenderer.h"
#include "BangEditor/UIInputColor.h"
#include "BangEditor/UIInputTexture.h"

namespace Bang
{
class IEventsValueChanged;
}

using namespace Bang;
using namespace BangEditor;

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
    p_imageInput->SetExtensions(Extensions::GetImageExtensions());

    p_tintInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_imageInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Image", p_imageInput);
    AddWidget("Tint", p_tintInput);

    SetLabelsWidth(70);
}

void CIWUIImageRenderer::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    if(!p_tintInput->HasFocus())
    {
        p_tintInput->SetColor(GetUIImageRenderer()->GetTint());
    }

    Texture2D *tex = GetUIImageRenderer()->GetImageTexture();
    p_imageInput->SetPath(tex ? tex->GetResourceFilepath() : Path::Empty);
}

UIImageRenderer *CIWUIImageRenderer::GetUIImageRenderer() const
{
    return SCAST<UIImageRenderer *>(GetComponent());
}

void CIWUIImageRenderer::OnValueChangedCIW(
    EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

    GetUIImageRenderer()->SetTint(p_tintInput->GetColor());
    GetUIImageRenderer()->SetImageTexture(p_imageInput->GetPath());
}
