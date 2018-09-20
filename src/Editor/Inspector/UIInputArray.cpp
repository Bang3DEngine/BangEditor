#include "BangEditor/UIInputArray.h"

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

    GameObjectFactory::CreateUIHSpacer()->SetParent(m_addNewElementRow);
    UIButton *addButton = GameObjectFactory::CreateUIButton("",
                                EditorTextureFactory::GetAddIcon());
    addButton->GetIcon()->SetTint(Color::Green.WithValue(0.75f));
    addButton->GetGameObject()->SetParent(m_addNewElementRow);
    addButton->AddClickedCallback([this]()
    {
        ASSERT(m_getNewElementFunction);
        GameObject *newElement = m_getNewElementFunction();
        AddElement(newElement);
    });

    MoveAddNewElementRowToEnd();
}

UIInputArray::~UIInputArray()
{
}

void UIInputArray::AddElement_(GameObject *widget,
                               uint index_,
                               bool propagateChangeEvent)
{
    if (!GetArray().Contains(widget))
    {
        uint index = (index_ == -1u) ? m_array.Size() : index_;
        UIInputArrayRow *row = GameObject::Create<UIInputArrayRow>();
        row->Init(this);
        row->SetContainedGameObject(widget);
        row->SetParent(this, index);
        m_array.Insert(widget, index);

        MoveAddNewElementRowToEnd();
        if (propagateChangeEvent)
        {
            EventEmitter<IEventsValueChanged>::PropagateToListeners(
                        &IEventsValueChanged::OnValueChanged, this);
        }
    }
}

void UIInputArray::RemoveElement_(GameObject *widget,
                                  bool propagateChangeEvent)
{
    if (GetArray().Contains(widget))
    {
        if (UIInputArrayRow *row = GetRowFromElement(widget))
        {
            m_array.Remove(widget);
            GameObject::Destroy(row);

            MoveAddNewElementRowToEnd();
            if (propagateChangeEvent)
            {
                EventEmitter<IEventsValueChanged>::PropagateToListeners(
                            &IEventsValueChanged::OnValueChanged, this);
            }
        }
    }
}

void UIInputArray::AddElement(GameObject *widget, uint index)
{
    AddElement_(widget, index, true);
}

void UIInputArray::RemoveElement(GameObject *widget)
{
    RemoveElement_(widget, true);
}

void UIInputArray::RemoveElement(uint index)
{
    ASSERT(index <= GetArray().Size());
    GameObject *widget = GetArray()[index];
    RemoveElement(widget);
}

void UIInputArray::RemoveRow(UIInputArrayRow *row, bool propagateChangeEvent)
{
    RemoveElement_(row->GetContainedGameObject(), propagateChangeEvent);
}

void UIInputArray::MoveRow(UIInputArrayRow *row, int displacement)
{
    int numChildren = SCAST<int>(GetChildren().Size() - 1);
    int oldIndex = row->GetIndexInsideParent();
    int newIndex = (oldIndex + displacement + numChildren) % numChildren;

    RemoveRow(row, false);
    AddElement_(row->GetContainedGameObject(), newIndex, false);
    EventEmitter<IEventsValueChanged>::PropagateToListeners(
                &IEventsValueChanged::OnValueChanged, this);
}

void UIInputArray::SetGetNewElementFunction(
                            UIInputArray::CreateNewElementFunction function)
{
    m_getNewElementFunction = function;
}

uint UIInputArray::Size() const
{
    return GetArray().Size();
}

const Array<GameObject*>& UIInputArray::GetArray() const
{
    return m_array;
}

void UIInputArray::MoveAddNewElementRowToEnd()
{
    m_addNewElementRow->SetParent(this, -1);
}

UIInputArrayRow *UIInputArray::GetRowFromElement(GameObject *element) const
{
    return element->GetParent() ? DCAST<UIInputArrayRow*>(element->GetParent()) :
                                  nullptr;
}
