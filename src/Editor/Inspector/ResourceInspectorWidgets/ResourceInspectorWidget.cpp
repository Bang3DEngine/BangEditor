#include "BangEditor/ResourceInspectorWidget.h"

#include "Bang/GameObject.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIImageRenderer.h"
#include "BangEditor/EditorTextureFactory.h"

using namespace Bang;
using namespace BangEditor;

void ResourceInspectorWidget::InitInnerWidgets()
{
    InspectorWidget::InitInnerWidgets();
    GetInspectorWidgetTitle()
        ->GetEnabledCheckBox()
        ->GetGameObject()
        ->SetEnabled(false);
    UpdateIcon();
}

void ResourceInspectorWidget::Update()
{
    InspectorWidget::Update();
}

void ResourceInspectorWidget::SetPath(const Path &path)
{
    if (path != GetPath())
    {
        m_path = path;
        UpdateFromFileWhenChanged();
    }
}

Path ResourceInspectorWidget::GetPath() const
{
    return m_path;
}

Texture2D *ResourceInspectorWidget::GetIconTexture() const
{
    return EditorTextureFactory::GetCheckerboard();
}

Color ResourceInspectorWidget::GetIconTint() const
{
    return Color::White();
}

void ResourceInspectorWidget::UpdateIcon()
{
    UIImageRenderer *icon = GetInspectorWidgetTitle()->GetIcon();
    icon->SetImageTexture(GetIconTexture());
    icon->SetTint(GetIconTint());
}
