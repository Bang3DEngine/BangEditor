#ifndef AESCONNECTIONPOINT_H
#define AESCONNECTIONPOINT_H

#include "Bang/Bang.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsFocus.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIFocusable;
FORWARD class LineRenderer;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class AESNode;

class AESConnectionPoint : public GameObject,
                           public EventListener<IEventsFocus>
{
    GAMEOBJECT_EDITOR(AESConnectionPoint);

public:
    enum class Type
    {
        IN,
        OUT
    };

    AESConnectionPoint();
    AESConnectionPoint(AESConnectionPoint::Type type);
    virtual ~AESConnectionPoint();

    // GameObject
    void Update() override;
    void BeforeRender() override;

    void SetNode(AESNode *node);
    void SetSiblingConnectionPoint(AESConnectionPoint *siblingConnectionPoint);

    void ConnectTo(AESConnectionPoint *toConnectionPoint);

    AESNode *GetNode() const;
    AESConnectionPoint::Type GetType() const;
    AESConnectionPoint *GetConnection() const;
    AESConnectionPoint *GetSiblingConnectionPoint() const;

private:
    AESNode *p_node = nullptr;
    Type m_type = Undef<Type>();
    AESConnectionPoint *p_siblingConnectionPoint = nullptr;
    AESConnectionPoint *p_pointConnectedTo = nullptr;
    AESConnectionPoint *p_currentDragPointConnectingTo = nullptr;

    UIImageRenderer *p_bg = nullptr;
    UIFocusable *p_focusable = nullptr;
    UIImageRenderer *p_border = nullptr;
    LineRenderer *p_connectionLR = nullptr;

    bool IsValidConnection(AESConnectionPoint *otherConnPoint) const;

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // AESCONNECTIONPOINT_H

