#include "BangEditor/UIInputArray.h"

#include "Bang/MetaNode.h"
#include "Bang/UIButton.h"
#include "Bang/TextureFactory.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/EditorTextureFactory.h"

USING_NAMESPACE_BANG_EDITOR

UIInputArray::UIInputArray()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();
    vl->SetChildrenHorizontalStretch(Stretch::FULL);
    vl->SetSpacing(5);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleWidth(1.0f);

    m_addNewElementRow = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *newRowHL = m_addNewElementRow->AddComponent<UIHorizontalLayout>();
    newRowHL->SetChildrenHorizontalAlignment(HorizontalAlignment::RIGHT);

    UIButton *addButton = GameObjectFactory::CreateUIButton("",
                                EditorTextureFactory::GetAddIcon());
    addButton->GetIcon()->SetTint(Color::Green.WithValue(0.75f));
    addButton->GetGameObject()->SetParent(m_addNewElementRow);
    addButton->AddClickedCallback([this]()
    {
        ASSERT(m_createNewInputGameObjectFunction);
        GameObject *newElement = m_createNewInputGameObjectFunction();
        AddInputGameObject(newElement);
    });

    GameObjectFactory::CreateUIHSpacer()->SetParent(m_addNewElementRow);

    MoveAddNewInputGameObjectRowToEnd();
}

UIInputArray::~UIInputArray()
{
}

void UIInputArray::AddInputGameObject_(GameObject *widget,
                                    uint index_,
                                    bool propagateChangeEvent)
{
    if (!GetInputGameObjectsArray().Contains(widget))
    {
        uint index = (index_ == -1u) ? m_inputGameObjects.Size() : index_;
        UIInputArrayRow *row = GameObject::Create<UIInputArrayRow>();
        row->Init(this);
        row->SetContainedGameObject(widget);
        row->SetParent(this, index);
        m_inputGameObjects.Insert(widget, index);

        MoveAddNewInputGameObjectRowToEnd();
        if (propagateChangeEvent)
        {
            EventEmitter<IEventsValueChanged>::PropagateToListeners(
                        &IEventsValueChanged::OnValueChanged, this);
        }
    }
}

void UIInputArray::RemoveInputGameObject_(GameObject *widget,
                                       bool propagateChangeEvent)
{
    if (GetInputGameObjectsArray().Contains(widget))
    {
        if (UIInputArrayRow *row = GetRowFromInputGameObject(widget))
        {
            m_inputGameObjects.Remove(widget);
            GameObject::Destroy(row);

            MoveAddNewInputGameObjectRowToEnd();
            if (propagateChangeEvent)
            {
                EventEmitter<IEventsValueChanged>::PropagateToListeners(
                            &IEventsValueChanged::OnValueChanged, this);
            }
        }
    }
}

void UIInputArray::UpdateElementsSerializable(
                    const Array<MetaNode> &referenceMetaNodes,
                    const Array<Serializable*> &serializablesToUpdate,
                    std::function<Serializable*()> createNewElementFunction,
                    std::function<void(Serializable*)> removeElementFunction)
{
    Array<Serializable*> serializablesToUpdateCopy = serializablesToUpdate;

    // Add or remove needed or unneeded elements (if any)
    while (referenceMetaNodes.Size() != serializablesToUpdateCopy.Size())
    {
        if (serializablesToUpdateCopy.Size() < referenceMetaNodes.Size())
        {
            Serializable *newSerializable = createNewElementFunction();
            serializablesToUpdateCopy.PushBack(newSerializable);
        }
        else
        {
            removeElementFunction(serializablesToUpdateCopy.Back());
            serializablesToUpdateCopy.PopBack();
        }
    }

    // Update all elements
    ASSERT(serializablesToUpdateCopy.Size() == referenceMetaNodes.Size());
    for (uint i = 0; i < serializablesToUpdateCopy.Size(); ++i)
    {
        ASSERT(i < referenceMetaNodes.Size());
        ASSERT(i < serializablesToUpdateCopy.Size());

        Serializable *serializableToUpdate = serializablesToUpdateCopy[i];
        const MetaNode &elementMeta = referenceMetaNodes[i];
        serializableToUpdate->ImportMeta(elementMeta);
    }
}

void UIInputArray::Clear()
{
    while (!GetInputGameObjectsArray().IsEmpty())
    {
        RemoveInputGameObject( GetInputGameObjectsArray().Size() - 1 );
    }
}

void UIInputArray::AddInputGameObject(GameObject *widget, uint index)
{
    AddInputGameObject_(widget, index, true);
}

void UIInputArray::UpdateInputGameObjects(const Array<MetaNode> &elementsMeta)
{
    ASSERT(m_createNewInputGameObjectFunction);
    UIInputArray::UpdateElementsSerializable(
                elementsMeta,
                m_inputGameObjects.To<Array, Serializable*>(),
                [this]()
                {
                    GameObject *newGo = m_createNewInputGameObjectFunction();
                    return SCAST<Serializable*>(newGo);
                },
                [this](Serializable *s)
                {
                    RemoveInputGameObjectSerializable(s);
                }
            );
}

void UIInputArray::RemoveInputGameObjectSerializable(Serializable *widget)
{
    RemoveInputGameObject(DCAST<GameObject*>(widget));
}

void UIInputArray::RemoveInputGameObject(GameObject *widget)
{
    RemoveInputGameObject_(widget, true);
}

void UIInputArray::RemoveInputGameObject(uint index)
{
    ASSERT(index <= GetInputGameObjectsArray().Size());
    GameObject *widget = GetInputGameObjectsArray()[index];
    RemoveInputGameObject(widget);
}

void UIInputArray::RemoveRow(UIInputArrayRow *row, bool propagateChangeEvent)
{
    RemoveInputGameObject_(row->GetContainedGameObject(), propagateChangeEvent);
}

void UIInputArray::MoveRow(UIInputArrayRow *row, int displacement)
{
    int numChildren = SCAST<int>(GetChildren().Size() - 1);
    int oldIndex = row->GetIndexInsideParent();
    int newIndex = (oldIndex + displacement + numChildren) % numChildren;

    RemoveRow(row, false);
    AddInputGameObject_(row->GetContainedGameObject(), newIndex, false);
    EventEmitter<IEventsValueChanged>::PropagateToListeners(
                &IEventsValueChanged::OnValueChanged, this);
}

void UIInputArray::SetCreateNewInputGameObjectFunction(
                        UIInputArray::CreateNewInputGameObjectFunction function)
{
    m_createNewInputGameObjectFunction = function;
}

uint UIInputArray::Size() const
{
    return GetInputGameObjectsArray().Size();
}

Array<MetaNode> UIInputArray::GetInputGameObjectsMetas() const
{
    Array<MetaNode> metas;

    for (GameObject *inputGo : GetInputGameObjectsArray())
    {
        metas.PushBack( inputGo->GetMeta() );
    }

    return metas;
}

const Array<GameObject*>& UIInputArray::GetInputGameObjectsArray() const
{
    return m_inputGameObjects;
}

void UIInputArray::MoveAddNewInputGameObjectRowToEnd()
{
    m_addNewElementRow->SetParent(this, -1);
}

UIInputArrayRow *UIInputArray::GetRowFromInputGameObject(GameObject *element) const
{
    return element->GetParent() ? DCAST<UIInputArrayRow*>(element->GetParent()) :
                                  nullptr;
}
