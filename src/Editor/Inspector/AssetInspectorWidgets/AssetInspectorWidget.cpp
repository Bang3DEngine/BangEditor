#include "BangEditor/AssetInspectorWidget.h"

#include "Bang/GameObject.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIImageRenderer.h"
#include "BangEditor/EditorTextureFactory.h"

using namespace Bang;
using namespace BangEditor;

void AssetInspectorWidget::InitInnerWidgets()
{
    InspectorWidget::InitInnerWidgets();
    GetInspectorWidgetTitle()
        ->GetEnabledCheckBox()
        ->GetGameObject()
        ->SetEnabled(false);
    UpdateIcon();
}

void AssetInspectorWidget::Update()
{
    InspectorWidget::Update();
}

void AssetInspectorWidget::SetPath(const Path &path)
{
    if (path != GetPath())
    {
        m_path = path;
        UpdateFromFileWhenChanged();
    }
}

Path AssetInspectorWidget::GetPath() const
{
    return m_path;
}

Texture2D *AssetInspectorWidget::GetIconTexture() const
{
    return EditorTextureFactory::GetCheckerboard();
}

Color AssetInspectorWidget::GetIconTint() const
{
    return Color::White();
}

void AssetInspectorWidget::UpdateIcon()
{
    UIImageRenderer *icon = GetInspectorWidgetTitle()->GetIcon();
    icon->SetImageTexture(GetIconTexture());
    icon->SetTint(GetIconTint());
}
