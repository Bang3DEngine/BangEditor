#ifndef AIWASSET_H
#define AIWASSET_H

#include "Bang/Asset.h"
#include "Bang/AssetHandle.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.h"
#include "Bang/IEventsAsset.h"
#include "Bang/MetaNode.h"
#include "Bang/Path.h"
#include "BangEditor/AssetInspectorWidget.h"

using namespace Bang;
namespace BangEditor
{
class UndoRedoFileChange;
class UndoRedoSerializableChange;

template <class T>
class AIWAsset : public AssetInspectorWidget, public EventListener<IEventsAsset>
{
protected:
    AIWAsset() = default;
    virtual ~AIWAsset() override = default;

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void Update() override;

    void SetAsset(AH<T> &asset);

    AH<T> GetAsset() const;

    void BeginUndoRedo();
    void EndUndoRedo();

    virtual void OnValueChangedAIWAsset(
        EventEmitter<IEventsValueChanged> *object) = 0;
    virtual void UpdateInputsFromAsset() = 0;
    virtual Array<Path> GetUndoRedoPaths() const;
    virtual void OnAssetSet();

    // AssetInspectorWidget
    virtual void UpdateFromFileWhenChanged() override;

private:
    AH<T> m_asset;

    Array<UndoRedoFileChange *> p_undoRedosFileChanges;
    Array<UndoRedoSerializableChange *> p_undoRedosSerializableChanges;

    // IEventsValueChanged
    void OnValueChanged(EventEmitter<IEventsValueChanged> *object) override;

    // IEventsAsset
    virtual void OnAssetChanged(Asset *asset) override;
};
}  // namespace BangEditor

#include "BangEditor/AIWAsset.tcc"

#endif  // AIWASSET_H
