#include "BangEditor/UIInputObject.h"

#include "Bang/Color.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/Stretch.h"
#include "Bang/UIButton.h"
#include "Bang/UIDragDroppable.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIInputText.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UITextRenderer.h"
#include "BangEditor/EditorDialog.h"
#include "BangEditor/HierarchyItem.h"

using namespace Bang;
using namespace BangEditor;

UIInputObject::UIInputObject()
{
    SetName("UIInputObject");
    GetOpenButton()->GetGameObject()->SetEnabled(false);
}

UIInputObject::~UIInputObject()
{
}

bool UIInputObject::CanDoZoom() const
{
    return (GetObject() != nullptr);
}

void UIInputObject::SetObject(Object *object)
{
    p_object = object;
}

void UIInputObject::SetAcceptedClassIdBegin(ClassIdType classIdBegin)
{
    m_acceptedClassIdBegin = classIdBegin;
}

void UIInputObject::SetAcceptedClassIdEnd(ClassIdType classIdEnd)
{
    m_acceptedClassIdEnd = classIdEnd;
}

ClassIdType UIInputObject::GetAcceptedClassIdBegin() const
{
    return m_acceptedClassIdBegin;
}

ClassIdType UIInputObject::GetAcceptedClassIdEnd() const
{
    return m_acceptedClassIdEnd;
}

Object *UIInputObject::GetObject() const
{
    return p_object;
}

bool UIInputObject::AcceptsDrag(EventEmitter<IEventsDragDrop> *dd_) const
{
    if (UIDragDroppable *dragDroppable = DCAST<UIDragDroppable *>(dd_))
    {
        if (HierarchyItem *hierarchyItem =
                DCAST<HierarchyItem *>(dragDroppable->GetGameObject()))
        {
            return true;
        }
    }
    return false;
}

void UIInputObject::OnDropped(EventEmitter<IEventsDragDrop> *dd_)
{
    if (UIDragDroppable *dragDroppable = DCAST<UIDragDroppable *>(dd_))
    {
        if (HierarchyItem *hierarchyItem =
                DCAST<HierarchyItem *>(dragDroppable->GetGameObject()))
        {
        }
    }
}

void UIInputObject::OnSearchButtonClicked()
{
    Object *openObject = nullptr;
    bool accepted;
    EditorDialog::GetObject("Get Object...",
                            GetAcceptedClassIdBegin(),
                            GetAcceptedClassIdEnd(),
                            &openObject,
                            &accepted);
    if (accepted)
    {
        SetObject(openObject);
    }
}

void UIInputObject::OnOpenButtonClicked()
{
    // Empty
}
