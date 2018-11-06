#include "BangEditor/UIInputArray.h"

#include "Bang/Alignment.h"
#include "Bang/Assert.h"
#include "Bang/Color.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/Serializable.h"
#include "Bang/Stretch.h"
#include "Bang/UIButton.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/UIInputArrayRow.h"

namespace Bang
{
class MetaNode;
}

using namespace BangEditor;

UIInputArray::UIInputArray()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();
    vl->SetChildrenHorizontalStretch(Stretch::FULL);
    vl->SetSpacing(5);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleWidth(1.0f);

    m_addNewRowButtonRow = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *newRowHL =
        m_addNewRowButtonRow->AddComponent<UIHorizontalLayout>();
    newRowHL->SetChildrenHorizontalAlignment(HorizontalAlignment::RIGHT);

    UIButton *addButton = GameObjectFactory::CreateUIButton(
        "", EditorTextureFactory::GetAddIcon());
    addButton->GetIcon()->SetTint(Color::Green().WithValue(0.75f));
    addButton->GetGameObject()->SetParent(m_addNewRowButtonRow);
    addButton->AddClickedCallback([this]() {
        ASSERT(m_createNewRowFunction);
        GameObject *newElement = m_createNewRowFunction();
        AddRow(newElement);
    });

    GameObjectFactory::CreateUIHSpacer()->SetParent(m_addNewRowButtonRow);

    MoveAddNewRowButtonRowToEnd();
}

UIInputArray::~UIInputArray()
{
}

UIInputArrayRow *UIInputArray::AddRow_(GameObject *rowGameObject,
                                       uint index_,
                                       bool propagateChangeEvent)
{
    if (!GetRowGameObjects().Contains(rowGameObject))
    {
        uint index =
            (index_ == SCAST<uint>(-1)) ? m_rowGameObjects.Size() : index_;
        UIInputArrayRow *row = new UIInputArrayRow();
        row->Init(this);
        row->SetContainedGameObject(rowGameObject);
        row->SetParent(this, index);
        m_rowGameObjects.Insert(rowGameObject, index);

        if (auto *ee =
                DCAST<EventEmitter<IEventsValueChanged> *>(rowGameObject))
        {
            ee->RegisterListener(this);
        }

        MoveAddNewRowButtonRowToEnd();
        if (propagateChangeEvent)
        {
            EventEmitter<IEventsValueChanged>::PropagateToListeners(
                &IEventsValueChanged::OnValueChanged, this);
        }
        return row;
    }
    return nullptr;
}

void UIInputArray::RemoveRow_(GameObject *rowGameObject,
                              bool propagateChangeEvent)
{
    if (GetRowGameObjects().Contains(rowGameObject))
    {
        if (UIInputArrayRow *row = GetRowFromRowGameObject(rowGameObject))
        {
            if (auto *ee =
                    DCAST<EventEmitter<IEventsValueChanged> *>(rowGameObject))
            {
                ee->RegisterListener(this);
            }

            m_rowGameObjects.Remove(rowGameObject);
            GameObject::Destroy(row);

            MoveAddNewRowButtonRowToEnd();
            if (propagateChangeEvent)
            {
                EventEmitter<IEventsValueChanged>::PropagateToListeners(
                    &IEventsValueChanged::OnValueChanged, this);
            }
        }
    }
}

void UIInputArray::UpdateSerializables(
    Array<Serializable *> serializablesToUpdate,
    Array<Serializable *> serializablesToUpdateFrom,
    std::function<Serializable *()> createNewSerializableFunction,
    std::function<void(Serializable *)> removeSerializableFunction,
    bool newCreatedSerializablesUpdateTheFromSerializable,
    bool updatingInputRows)
{
    // Add or remove needed or unneeded elements (if any)
    uint prevSerializablesToUpdateSize = serializablesToUpdate.Size();
    while (serializablesToUpdateFrom.Size() != serializablesToUpdate.Size())
    {
        if (serializablesToUpdate.Size() < serializablesToUpdateFrom.Size())
        {
            Serializable *newSerializable = createNewSerializableFunction();
            serializablesToUpdate.PushBack(newSerializable);
        }
        else
        {
            removeSerializableFunction(serializablesToUpdate.Back());
            serializablesToUpdate.PopBack();
        }
    }

    // Update all non-new elements
    ASSERT(serializablesToUpdate.Size() == serializablesToUpdateFrom.Size());
    for (uint i = 0; i < serializablesToUpdate.Size(); ++i)
    {
        ASSERT(i < serializablesToUpdate.Size());
        ASSERT(i < serializablesToUpdateFrom.Size());

        Serializable *serializableToUpdate = serializablesToUpdate[i];
        Serializable *serializableToUpdateFrom = serializablesToUpdateFrom[i];

        bool isNewElement = (i >= prevSerializablesToUpdateSize);
        if (isNewElement && newCreatedSerializablesUpdateTheFromSerializable)
        {
            std::swap(serializableToUpdate, serializableToUpdateFrom);
        }

        MetaNode updateFromMeta = serializableToUpdateFrom->GetMeta();
        serializableToUpdate->ImportMeta(updateFromMeta);

        EventEmitter<IEventsUIInputArray>::PropagateToListeners(
            (updatingInputRows ? &IEventsUIInputArray::OnInputRowUpdated
                               : &IEventsUIInputArray::OnReferenceUpdated),
            this,
            serializableToUpdate,
            serializableToUpdateFrom);
    }
}

void UIInputArray::Clear(bool synchronize)
{
    if (!synchronize)
    {
        EventEmitter<IEventsValueChanged>::SetEmitEvents(false);
    }

    while (!GetRowGameObjects().IsEmpty())
    {
        RemoveRow(GetRowGameObjects().Size() - 1);
    }

    EventEmitter<IEventsValueChanged>::SetEmitEvents(true);
}

void UIInputArray::AddRow(GameObject *rowGameObject, uint index)
{
    AddRow_(rowGameObject, index, true);
}

void UIInputArray::RemoveRow(GameObject *rowGameObject)
{
    RemoveRow_(rowGameObject, true);
}

void UIInputArray::RemoveRow(uint index)
{
    ASSERT(index <= GetRowGameObjects().Size());
    GameObject *rowGameObject = GetRowGameObjects()[index];
    RemoveRow(rowGameObject);
}

void UIInputArray::MoveRow(UIInputArrayRow *row, int displacement)
{
    int numChildren = SCAST<int>(GetChildren().Size() - 1);
    int oldIndex = row->GetIndexInsideParent();
    int newIndex = (oldIndex + displacement + numChildren) % numChildren;

    ASSERT(m_moveReferenceFunction);
    m_moveReferenceFunction(oldIndex, newIndex);

    RemoveRow(row, false);
    AddRow_(row->GetContainedGameObject(), newIndex, false);

    EventEmitter<IEventsUIInputArray>::PropagateToListeners(
        &IEventsUIInputArray::OnInputRowMoved,
        this,
        row->GetContainedGameObject(),
        oldIndex,
        newIndex);
    EventEmitter<IEventsValueChanged>::PropagateToListeners(
        &IEventsValueChanged::OnValueChanged, this);
}

void UIInputArray::RemoveRow(UIInputArrayRow *row, bool propagateChangeEvent)
{
    RemoveRow_(row->GetContainedGameObject(), propagateChangeEvent);
}

uint UIInputArray::Size() const
{
    return GetRowGameObjects().Size();
}

const Array<GameObject *> &UIInputArray::GetRowGameObjects() const
{
    return m_rowGameObjects;
}

void UIInputArray::MoveAddNewRowButtonRowToEnd()
{
    m_addNewRowButtonRow->SetParent(this, -1);
}

UIInputArrayRow *UIInputArray::GetRowFromRowGameObject(
    GameObject *rowGameObject) const
{
    return rowGameObject->GetParent()
               ? DCAST<UIInputArrayRow *>(rowGameObject->GetParent())
               : nullptr;
}

void UIInputArray::OnValueChanged(EventEmitter<IEventsValueChanged> *)
{
    EventEmitter<IEventsValueChanged>::PropagateToListeners(
        &IEventsValueChanged::OnValueChanged, this);
}
