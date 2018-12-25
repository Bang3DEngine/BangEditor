#ifndef AIWMODEL_H
#define AIWMODEL_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/AssetHandle.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/IEvents.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/Model.h"
#include "Bang/Path.h"
#include "Bang/String.h"
#include "BangEditor/AIWAsset.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class IEventsValueChanged;
class Texture2D;
class UIInputNumber;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class PreviewViewer;

class AIWModel : public AIWAsset<Model>
{
    GAMEOBJECT_EDITOR(AIWModel);

public:
    // InspectorWidget
    void Init() override;

private:
    PreviewViewer *p_modelPreviewViewer = nullptr;

    UIInputNumber *p_numMeshes = nullptr;
    UIInputNumber *p_numVertices = nullptr;
    UIInputNumber *p_numTriangles = nullptr;
    UIInputNumber *p_numBones = nullptr;
    UIInputNumber *p_numMaterials = nullptr;
    UIInputNumber *p_numAnimations = nullptr;

    AIWModel();
    virtual ~AIWModel() override;

    Model *GetModel() const;

    // AIWAsset
    void OnAssetSet() override;
    void UpdateInputsFromAsset() override;
    Texture2D *GetIconTexture() const override;

    // AIWAsset
    void OnValueChangedAIWAsset(
        EventEmitter<IEventsValueChanged> *object) override;

    friend class AssetInspectorWidgetFactory;
};
}

#endif  // AIWMODEL_H
