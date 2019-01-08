#ifndef AIWMESH_H
#define AIWMESH_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/AssetHandle.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/IEvents.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/Mesh.h"
#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/UIInputNumber.h"
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

class AIWMesh : public AIWAsset<Mesh>
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(AIWMesh);

public:
    // InspectorWidget
    void Init() override;

private:
    PreviewViewer *p_meshPreviewViewer = nullptr;
    UIInputNumber *p_numVertices = nullptr;
    UIInputNumber *p_numTriangles = nullptr;
    UIInputNumber *p_numBones = nullptr;

    AIWMesh();
    virtual ~AIWMesh() override;

    Mesh *GetMesh() const;

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

#endif  // AIWMESH_H
