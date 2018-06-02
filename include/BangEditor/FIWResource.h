#ifndef FIWRESOURCE_H
#define FIWRESOURCE_H

#include "Bang/Path.h"
#include "Bang/XMLNode.h"
#include "Bang/Resource.h"
#include "Bang/EventEmitter.h"
#include "Bang/ResourceHandle.h"
#include "Bang/IEventsResource.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/FileInspectorWidget.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UndoRedoFileChange;
FORWARD class UndoRedoSerializableChange;

template <class T>
class FIWResource : public FileInspectorWidget,
                    public EventListener<IEventsValueChanged>,
                    public EventListener<IEventsResource>
{
protected:
    FIWResource() = default;
    virtual ~FIWResource() = default;

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void Update() override;

    void SetResource(RH<T> &resource);

    RH<T> GetResource() const;

    void BeginUndoRedo();
    void EndUndoRedo();

    virtual void OnValueChangedFIWResource(
                        EventEmitter<IEventsValueChanged> *object) = 0;
    virtual void UpdateInputsFromResource() = 0;
    virtual Array<Path> GetUndoRedoPaths() const;

    // FileInspectorWidget
    virtual void UpdateFromFileWhenChanged() override;

private:
    RH<T> m_resource;

    XMLNode m_undoSerializableXMLBefore;
    Array<UndoRedoFileChange*> p_undoRedosFileChanges;

    // IEventsValueChanged
    void OnValueChanged(EventEmitter<IEventsValueChanged> *object) override;

    // IEventsResource
    virtual void OnResourceChanged(Resource *res) override;
};

NAMESPACE_BANG_EDITOR_END

#include "BangEditor/FIWResource.tcc"

#endif // FIWRESOURCE_H

