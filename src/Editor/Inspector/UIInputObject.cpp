#include "BangEditor/UIInputObject.h"

#include "Bang/Color.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/Scene.h"
#include "Bang/Stretch.h"
#include "Bang/UIButton.h"
#include "Bang/UIDragDroppable.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIInputText.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UITextRenderer.h"
#include "BangEditor/Editor.h"
#include "BangEditor/EditorDialog.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/Hierarchy.h"
#include "BangEditor/HierarchyItem.h"
#include "BangEditor/ScenePlayer.h"

using namespace Bang;
using namespace BangEditor;

UIInputObject::UIInputObject()
{
    SetName("UIInputObject");
    SetZoomable(false);
}

UIInputObject::~UIInputObject()
{
}

bool UIInputObject::CanDoZoom() const
{
    return (GetObject() != nullptr);
}

void UIInputObject::SetObject(Object *object_)
{
    Object *object = object_;
    if (object_ &&
        !IsSubClass(
            GetAcceptedClassIdBegin(), GetAcceptedClassIdEnd(), object_))
    {
        object = GetAcceptedObjectIn(GetGameObjectOf(object_));
    }

    if (object != GetObject())
    {
        m_objectPtr.SetObject(object);

        String textContent = "None";
        if (object)
        {
            if (GameObject *go = DCAST<GameObject *>(object))
            {
                textContent = go->GetName();
            }
            else if (Component *comp = DCAST<Component *>(object))
            {
                textContent = (comp->GetGameObject()->GetName() + "/" +
                               comp->GetClassName());
            }
        }
        GetInputText()->GetText()->SetContent(textContent);

        EventEmitter<IEventsValueChanged>::PropagateToListeners(
            &IEventsValueChanged::OnValueChanged, this);
    }
}

void UIInputObject::SetGUID(const GUID &guid)
{
    Scene *sceneToLookObjectIn = GetSceneToLookObjectIn();
    Object *object = sceneToLookObjectIn
                         ? sceneToLookObjectIn->FindObjectInDescendants(guid)
                         : nullptr;
    SetObject(object);
}

void UIInputObject::SetAcceptedClassIdBeginAndEnd(ClassIdType classIdBegin,
                                                  ClassIdType classIdEnd)
{
    if (classIdBegin != GetAcceptedClassIdBegin() ||
        classIdEnd != GetAcceptedClassIdEnd())
    {
        m_acceptedClassIdBegin = classIdBegin;
        m_acceptedClassIdEnd = classIdEnd;

        GameObject *currentGo = GetGameObjectOf(GetObject());
        SetObject(GetAcceptedObjectIn(currentGo));
    }
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
    Scene *openScene = GetSceneToLookObjectIn();
    return m_objectPtr.GetObjectIn(openScene);
}

const ObjectPtr &UIInputObject::GetObjectPtr() const
{
    return m_objectPtr;
}

GUID UIInputObject::GetGUID() const
{
    return GetObject() ? GetObject()->GetGUID() : GUID::Empty();
}

Object *UIInputObject::GetAcceptedObjectIn(GameObject *go) const
{
    Object *object = nullptr;
    if (go)
    {
        object = go->FindObjectInDescendants(GetAcceptedClassIdBegin(),
                                             GetAcceptedClassIdEnd());
    }
    return object;
}

Object *UIInputObject::GetObjectInDragDroppable(
    EventEmitter<IEventsDragDrop> *dd) const
{
    if (UIDragDroppable *dragDroppable = DCAST<UIDragDroppable *>(dd))
    {
        if (HierarchyItem *hierarchyItem =
                dragDroppable->GetGameObject()
                    ->FindObjectInDescendants<HierarchyItem>())
        {
            GameObject *itemGo = hierarchyItem->GetReferencedGameObject();
            Object *object = GetAcceptedObjectIn(itemGo);
            return object;
        }
    }
    return nullptr;
}

bool UIInputObject::AcceptsDrag(EventEmitter<IEventsDragDrop> *dd) const
{
    return (GetObjectInDragDroppable(dd) != nullptr);
}

void UIInputObject::OnDropped(EventEmitter<IEventsDragDrop> *dd)
{
    if (Object *draggedObject = GetObjectInDragDroppable(dd))
    {
        SetObject(draggedObject);
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
    if (GameObject *objGo = GetGameObjectOf(GetObject()))
    {
        Hierarchy::GetInstance()->OnGameObjectSelected(objGo);
    }
}

Scene *UIInputObject::GetSceneToLookObjectIn() const
{
    Scene *sceneToLookObjectIn = nullptr;
    if (Editor::IsEditingScene())
    {
        sceneToLookObjectIn = EditorSceneManager::GetOpenScene();
    }
    else
    {
        ScenePlayer *sp = ScenePlayer::GetInstance();
        sceneToLookObjectIn = sp->GetPlayOpenScene();
    }
    return sceneToLookObjectIn;
}

GameObject *UIInputObject::GetGameObjectOf(Object *object) const
{
    GameObject *currentGo = nullptr;
    if (object)
    {
        currentGo = DCAST<GameObject *>(object);
        if (!currentGo)
        {
            if (Component *comp = DCAST<Component *>(object))
            {
                currentGo = comp->GetGameObject();
            }
        }
    }
    return currentGo;
}
