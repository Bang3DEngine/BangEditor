#include "BangEditor/AssetInspectorWidget.h"

#include "Bang/GameObject.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIImageRenderer.h"
#include "BangEditor/EditorTextureFactory.h"

using namespace Bang;
using namespace BangEditor;

void AssetInspectorWidget::InitInnerWidgets()
{
    SerializableInspectorWidget::InitInnerWidgets();
    GetInspectorWidgetTitle()
        ->GetEnabledCheckBox()
        ->GetGameObject()
        ->SetEnabled(false);
    UpdateIcon();
}

void AssetInspectorWidget::Update()
{
    SerializableInspectorWidget::Update();
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

void AssetInspectorWidget::UpdateFromFileWhenChanged()
{
    SerializableInspectorWidget::UpdateFromReference();
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
