#ifndef AESNODE_H
#define AESNODE_H

#include "Bang/Bang.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsFocus.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIFocusable;
FORWARD class UITextRenderer;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIContextMenu;
FORWARD class AESConnectionLine;

class AESNode : public GameObject,
                public EventListener<IEventsFocus>,
                public EventListener<IEventsDestroy>
{
    GAMEOBJECT_EDITOR(AESNode);

public:
	AESNode();
    virtual ~AESNode();

    // GameObject
    void Update() override;

    void OnZoomScaleChanged(float zoomScale);
    static void Connect(AESNode *fromNode, AESNode *toNode);

    UIFocusable* GetFocusable() const;

private:
    UIImageRenderer *p_bg = nullptr;
    UIFocusable *p_focusable = nullptr;
    UIImageRenderer *p_border = nullptr;
    UITextRenderer *p_nameText = nullptr;
    UIContextMenu *p_contextMenu = nullptr;

    Array<AESNode*> p_toConnectedNodes;
    Array<AESConnectionLine*> p_toConnectionLines;
    Map<AESConnectionLine*, AESNode*> p_toConnectionLineToConnectedNode;

    int m_framesPassedSinceLineDragStarted = 0;
    AESConnectionLine *p_toConnectionLineBeingDragged = nullptr;

    void CreateTransition();
    void OnDragConnectionLineEnd();

    // IEventsDestroy
    virtual void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // AESNODE_H

