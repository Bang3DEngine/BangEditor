#include "BangEditor/AIWMaterial.h"

#include "Bang/AssetHandle.h"
#include "Bang/Assets.h"
#include "Bang/Assets.tcc"
#include "Bang/EventEmitter.h"
#include "Bang/Extensions.h"
#include "Bang/Flags.h"
#include "Bang/GL.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/Material.h"
#include "Bang/NeededUniformFlags.h"
#include "Bang/Path.h"
#include "Bang/RenderPass.h"
#include "Bang/Shader.h"
#include "Bang/ShaderProgram.h"
#include "Bang/ShaderProgramFactory.h"
#include "Bang/Texture2D.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UISlider.h"
#include "BangEditor/AIWAsset.tcc"
#include "BangEditor/MaterialPreviewFactory.h"
#include "BangEditor/PreviewViewer.h"
#include "BangEditor/UIInputColor.h"
#include "BangEditor/UIInputFile.h"
#include "BangEditor/UIInputVector.h"

namespace Bang
{
class IEventsValueChanged;
}

namespace BangEditor
{
struct AssetPreviewFactoryParameters;
}

using namespace Bang;
using namespace BangEditor;

AIWMaterial::AIWMaterial()
{
}

AIWMaterial::~AIWMaterial()
{
}

void AIWMaterial::Init()
{
    AIWAsset<Material>::Init();

    SetName("AIWMaterial");
    SetTitle("Material");

    p_materialPreviewViewer = new PreviewViewer();

    AddWidget(p_materialPreviewViewer, 256);
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
}

Material *AIWMaterial::GetMaterial() const
{
    return SCAST<Material *>(GetAsset().Get());
}

void AIWMaterial::UpdateInputsFromAsset()
{
    p_materialPreviewViewer->SetPreviewImageProvider(
        [this](const AssetPreviewFactoryParameters &params) {
            return MaterialPreviewFactory::GetPreviewTextureFor(GetMaterial(),
                                                                params);
        });
}

Texture2D *AIWMaterial::GetIconTexture() const
{
    return MaterialPreviewFactory::GetPreviewTextureFor(GetMaterial()).Get();
}

void AIWMaterial::OnValueChangedAIWAsset(EventEmitter<IEventsValueChanged> *)
{
}
