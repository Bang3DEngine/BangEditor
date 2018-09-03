#include "BangEditor/RIWResource.h"

#include "Bang/Resources.h"
#include "Bang/MetaFilesManager.h"

#include "BangEditor/UndoRedoManager.h"
#include "BangEditor/UndoRedoFileChange.h"
#include "BangEditor/UndoRedoSerializableChange.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

template <class T>
void RIWResource<T>::InitInnerWidgets()
{
    ResourceInspectorWidget::InitInnerWidgets();
}

template <class T>
void RIWResource<T>::Update()
{
    ResourceInspectorWidget::Update();
}

template <class T>
void RIWResource<T>::SetResource(RH<T> &resource)
{
    if (resource != GetResource())
    {
        if (GetResource())
        {
            GetResource().Get()->EventEmitter<IEventsResource>::
                                 UnRegisterListener(this);
        }

        m_resource = resource;
        if (GetResource())
        {
            GetResource().Get()->EventEmitter<IEventsResource>::
                                 RegisterListener(this);
            OnResourceChanged( GetResource().Get() );
        }
    }
}

template <class T>
RH<T> RIWResource<T>::GetResource() const
{
    return m_resource;
}

template <class T>
void RIWResource<T>::BeginUndoRedo()
{
    if (GetResource())
    {
        if (GetPath().IsFile())
        {
            Array<Path> undoRedoPaths = GetUndoRedoPaths();
            for (const Path &undoRedoPath : undoRedoPaths)
            {
                UndoRedoFileChange *undoRedoFileChange =
                                        new UndoRedoFileChange(undoRedoPath);
                undoRedoFileChange->ReadBefore();
                p_undoRedosFileChanges.PushBack(undoRedoFileChange);
            }
        }

        UndoRedoSerializableChange *undoRedoSerializableChange =
           new UndoRedoSerializableChange(GetResource().Get(),
                                          GetResource().Get()->GetMeta(),
                                          MetaNode());
        p_undoRedosSerializableChanges.PushBack(undoRedoSerializableChange);
    }
}

template <class T>
void RIWResource<T>::EndUndoRedo()
{
    if (GetResource())
    {
        Array<UndoRedoAction*> undoRedoActions;
        for(UndoRedoFileChange *undoRedoFileChange : p_undoRedosFileChanges)
        {
            undoRedoFileChange->ReadAfter();
        }
        for(UndoRedoSerializableChange *undoRedoSerializableChange :
            p_undoRedosSerializableChanges)
        {
            undoRedoSerializableChange->SetMetaAfter(
                        GetResource().Get()->GetMeta() );
        }
        undoRedoActions.PushBack(p_undoRedosSerializableChanges);
        undoRedoActions.PushBack(p_undoRedosFileChanges);

        UndoRedoManager::PushActionsInSameStep(undoRedoActions);
        p_undoRedosSerializableChanges.Clear();
        p_undoRedosFileChanges.Clear();
    }
}

template <class T>
Array<Path> RIWResource<T>::GetUndoRedoPaths() const
{
    if (Resource *res = GetResource().Get())
    {
        return { MetaFilesManager::GetMetaFilepath(res->GetGUID()) };
    }
    return {};
}

template<class T>
void RIWResource<T>::UpdateFromFileWhenChanged()
{
    RH<T> newResourceT;
    if (GetPath().IsFile() || Resources::IsEmbeddedResource(GetPath()))
    {
        newResourceT = Resources::Load<T>( GetPath() );
    }
    SetResource(newResourceT);
}

template<class T>
void RIWResource<T>::OnValueChanged(EventEmitter<IEventsValueChanged> *object)
{
    if (GetResource())
    {
        BeginUndoRedo();

        // Virtual call
        OnValueChangedRIWResource(object);

        // Export to file
        Resource *resourceToExport = GetResource().Get();
        Path resourcePath = resourceToExport->GetResourceFilepath();
        if (!resourcePath.IsFile())
        {
            if (Resource *parentResource = GetResource().Get()->
                                           GetParentResource())
            {
                resourceToExport = parentResource;
                resourcePath = parentResource->GetResourceFilepath();
            }
        }

        Path importPath = MetaFilesManager::GetMetaFilepath(resourcePath);
        if (resourceToExport && importPath.IsFile())
        {
            resourceToExport->ExportMetaToFile(importPath);
        }

        EndUndoRedo();
    }
}

template <class T>
void RIWResource<T>::OnResourceChanged(Resource *res)
{
    ASSERT(res == GetResource().Get());
    if (GetResource())
    {
        IEventListenerCommon::SetReceiveEventsCommon(false);
        UpdateInputsFromResource();
        IEventListenerCommon::SetReceiveEventsCommon(true);
    }
}

