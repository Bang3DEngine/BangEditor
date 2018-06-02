#include "BangEditor/FIWResource.h"

#include "Bang/Resources.h"
#include "Bang/ImportFilesManager.h"

#include "BangEditor/UndoRedoManager.h"
#include "BangEditor/UndoRedoFileChange.h"
#include "BangEditor/UndoRedoSerializableChange.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

template <class T>
void FIWResource<T>::InitInnerWidgets()
{
    FileInspectorWidget::InitInnerWidgets();
}

template <class T>
void FIWResource<T>::Update()
{
    FileInspectorWidget::Update();
}

template <class T>
void FIWResource<T>::SetResource(RH<T> &resource)
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
RH<T> FIWResource<T>::GetResource() const
{
    return m_resource;
}

template <class T>
void FIWResource<T>::BeginUndoRedo()
{
    if (GetResource())
    {
        Resource *res = GetResource().Get();
        m_undoSerializableXMLBefore = res->GetXMLInfo();

        Array<Path> undoRedoPaths = GetUndoRedoPaths();
        for (const Path &undoRedoPath : undoRedoPaths)
        {
            UndoRedoFileChange *undoRedoFileChange =
                                    new UndoRedoFileChange(undoRedoPath);
            undoRedoFileChange->ReadBefore();
            p_undoRedosFileChanges.PushBack(undoRedoFileChange);
        }
    }
}

template <class T>
void FIWResource<T>::EndUndoRedo()
{
    if (GetResource())
    {
        Array<UndoRedoAction*> undoRedoActions;
        undoRedoActions.PushBack(
            new UndoRedoSerializableChange(GetResource().Get(),
                                           m_undoSerializableXMLBefore,
                                           GetResource().Get()->GetXMLInfo()));

        for(UndoRedoFileChange *undoRedoFileChange : p_undoRedosFileChanges)
        {
            undoRedoFileChange->ReadAfter();
        }
        undoRedoActions.PushBack(p_undoRedosFileChanges);

        UndoRedoManager::PushActionsInSameStep(undoRedoActions);

        p_undoRedosFileChanges.Clear();
    }
}

template <class T>
Array<Path> FIWResource<T>::GetUndoRedoPaths() const
{
    Resource *res = GetResource().Get();
    if (!res) { return {}; }
    return { ImportFilesManager::GetImportFilepath(res->GetGUID()) };
}

template<class T>
void FIWResource<T>::UpdateFromFileWhenChanged()
{
    RH<T> newResourceT;
    if (GetPath().IsFile())
    {
        newResourceT = Resources::Load<T>( GetPath() );
    }
    SetResource(newResourceT);
}

template<class T>
void FIWResource<T>::OnValueChanged(EventEmitter<IEventsValueChanged> *object)
{
    if (GetResource())
    {
        BeginUndoRedo();

        // Virtual call
        OnValueChangedFIWResource(object);

        // Export to file
        const Path importPath = ImportFilesManager::GetImportFilepath(
                                    GetResource().Get()->GetGUID());
        if (importPath.IsFile())
        {
            GetResource().Get()->ExportXMLToFile(importPath);
        }

        EndUndoRedo();
    }
}

template <class T>
void FIWResource<T>::OnResourceChanged(Resource *res)
{
    ASSERT(res == GetResource().Get());
    if (GetResource())
    {
        IEventListenerCommon::SetReceiveEventsCommon(false);
        UpdateInputsFromResource();
        IEventListenerCommon::SetReceiveEventsCommon(true);
    }
}

