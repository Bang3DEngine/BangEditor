#ifndef EDITORDRAGDROPMANAGER_H
#define EDITORDRAGDROPMANAGER_H

#include "Bang/Bang.h"
#include "Bang/GameObject.h"
#include "Bang/IDragDropListener.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class IEditorDragDropManagerListener : public virtual IEventListener
{
    EVENTLISTENER(IEditorDragDropManagerListener);

public:
    virtual void OnExplorerDragStart(const Path &path) { (void)path; }
    virtual void OnExplorerDragUpdate(const Path &path) { (void)path; }
    virtual void OnExplorerDrop(const Path &path) { (void)path; }
};



class EditorDragDropManager : public GameObject,
                              public IDragDropListener,
                              public EventEmitter<IEditorDragDropManagerListener>
{
    GAMEOBJECT_EDITOR(EditorDragDropManager);

public:
    static EditorDragDropManager *GetInstance();

private:
	EditorDragDropManager();
	virtual ~EditorDragDropManager();

    // IDragDropListener
    void OnDragStarted(UIDragDroppable *dragDroppable) override;
    void OnDragUpdate(UIDragDroppable *dragDroppable) override;
    void OnDrop(UIDragDroppable *dragDroppable) override;

    friend class EditorScene;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORDRAGDROPMANAGER_H

