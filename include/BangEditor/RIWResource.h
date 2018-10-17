#ifndef RIWRESOURCE_H
#define RIWRESOURCE_H

#include "Bang/EventEmitter.h"
#include "Bang/IEventsResource.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/MetaNode.h"
#include "Bang/Path.h"
#include "Bang/Resource.h"
#include "Bang/ResourceHandle.h"

#include "BangEditor/ResourceInspectorWidget.h"

using namespace Bang;
namespace BangEditor
{
class UndoRedoFileChange;
class UndoRedoSerializableChange;

template <class T>
class RIWResource : public ResourceInspectorWidget,
                    public EventListener<IEventsValueChanged>,
                    public EventListener<IEventsResource>
{
protected:
    RIWResource() = default;
    virtual ~RIWResource() = default;

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void Update() override;

    void SetResource(RH<T> &resource);

    RH<T> GetResource() const;

    void BeginUndoRedo();
    void EndUndoRedo();

    virtual void OnValueChangedRIWResource(
        EventEmitter<IEventsValueChanged> *object) = 0;
    virtual void UpdateInputsFromResource() = 0;
    virtual Array<Path> GetUndoRedoPaths() const;
    virtual void OnResourceSet();

    // ResourceInspectorWidget
    virtual void UpdateFromFileWhenChanged() override;

private:
    RH<T> m_resource;

    Array<UndoRedoFileChange *> p_undoRedosFileChanges;
    Array<UndoRedoSerializableChange *> p_undoRedosSerializableChanges;

    // IEventsValueChanged
    void OnValueChanged(EventEmitter<IEventsValueChanged> *object) override;

    // IEventsResource
    virtual void OnResourceChanged(Resource *res) override;
};
}

#include "BangEditor/RIWResource.tcc"

#endif  // RIWRESOURCE_H
