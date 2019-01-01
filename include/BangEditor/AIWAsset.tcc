#pragma once

#include "Bang/Assets.h"
#include "Bang/MetaFilesManager.h"
#include "Bang/UIImageRenderer.h"
#include "BangEditor/AIWAsset.h"
#include "BangEditor/UndoRedoFileChange.h"
#include "BangEditor/UndoRedoManager.h"
#include "BangEditor/UndoRedoSerializableChange.h"

using namespace Bang;
namespace BangEditor
{
template <class T>
void AIWAsset<T>::InitInnerWidgets()
{
    AssetInspectorWidget::InitInnerWidgets();
}

template <class T>
void AIWAsset<T>::Update()
{
    AssetInspectorWidget::Update();
}

template <class T>
void AIWAsset<T>::SetAsset(AH<T> &asset)
{
    if (asset != GetAsset())
    {
        if (GetAsset())
        {
            GetAsset().Get()->EventEmitter<IEventsAsset>::UnRegisterListener(
                this);
        }

        m_asset = asset;
        if (GetAsset())
        {
            GetAsset().Get()->EventEmitter<IEventsAsset>::RegisterListener(
                this);

            UpdateIcon();
            OnAssetChanged(GetAsset().Get());
            OnAssetSet();
        }

        SetSerializable(GetAsset().Get());
    }
}

template <class T>
AH<T> AIWAsset<T>::GetAsset() const
{
    return m_asset;
}

template <class T>
void AIWAsset<T>::BeginUndoRedo()
{
    if (GetAsset())
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
            new UndoRedoSerializableChange(
                GetAsset().Get(), GetAsset().Get()->GetMeta(), MetaNode());
        p_undoRedosSerializableChanges.PushBack(undoRedoSerializableChange);
    }
}

template <class T>
void AIWAsset<T>::EndUndoRedo()
{
    if (GetAsset())
    {
        Array<UndoRedoAction *> undoRedoActions;
        for (UndoRedoFileChange *undoRedoFileChange : p_undoRedosFileChanges)
        {
            undoRedoFileChange->ReadAfter();
        }
        for (UndoRedoSerializableChange *undoRedoSerializableChange :
             p_undoRedosSerializableChanges)
        {
            undoRedoSerializableChange->SetMetaAfter(
                GetAsset().Get()->GetMeta());
        }
        undoRedoActions.PushBack(p_undoRedosSerializableChanges);
        undoRedoActions.PushBack(p_undoRedosFileChanges);

        UndoRedoManager::PushActionsInSameStep(undoRedoActions);
        p_undoRedosSerializableChanges.Clear();
        p_undoRedosFileChanges.Clear();
    }
}

template <class T>
void AIWAsset<T>::OnValueChangedAIWAsset(EventEmitter<IEventsValueChanged> *)
{
}

template <class T>
void AIWAsset<T>::UpdateInputsFromAsset()
{
}

template <class T>
Array<Path> AIWAsset<T>::GetUndoRedoPaths() const
{
    if (Asset *res = GetAsset().Get())
    {
        return {MetaFilesManager::GetMetaFilepath(res->GetGUID())};
    }
    return {};
}

template <class T>
void AIWAsset<T>::OnAssetSet()
{
    // Empty
}

template <class T>
void AIWAsset<T>::UpdateFromFileWhenChanged()
{
    AH<T> newAssetT;
    if (GetPath().IsFile() || Assets::IsEmbeddedAsset(GetPath()))
    {
        newAssetT = Assets::Load<T>(GetPath());
    }
    SetAsset(newAssetT);
}

template <class T>
void AIWAsset<T>::OnValueChanged(
    Bang::EventEmitter<IEventsValueChanged> *object)
{
    AssetInspectorWidget::OnValueChanged(object);

    if (GetAsset())
    {
        BeginUndoRedo();

        // Virtual call
        OnValueChangedAIWAsset(object);

        // Export to file
        Asset *assetToExport = GetAsset().Get();
        Path AssetPath = assetToExport->GetAssetFilepath();
        if (!AssetPath.IsFile())
        {
            if (Asset *parentAsset = GetAsset().Get()->GetParentAsset())
            {
                assetToExport = parentAsset;
                AssetPath = parentAsset->GetAssetFilepath();
            }
        }

        Path importPath = MetaFilesManager::GetMetaFilepath(AssetPath);
        if (assetToExport && importPath.IsFile())
        {
            assetToExport->ExportMetaToFile(importPath);
        }

        EndUndoRedo();
    }
}

template <class T>
void AIWAsset<T>::OnAssetChanged(Asset *asset)
{
    ASSERT(asset == GetAsset().Get());
    if (GetAsset())
    {
        IEventListenerCommon::SetReceiveEventsCommon(false);
        UpdateInputsFromAsset();
        IEventListenerCommon::SetReceiveEventsCommon(true);
    }
}
}  // namespace BangEditor
