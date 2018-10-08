#ifndef AESCONNECTIONLINE_H
#define AESCONNECTIONLINE_H

#include "Bang/Bang.h"
#include "Bang/GameObject.h"
#include "Bang/LineRenderer.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class AESNode;
FORWARD class UIContextMenu;

class AESConnectionLine : public GameObject
{
    GAMEOBJECT_EDITOR(AESConnectionLine);

public:
	AESConnectionLine();
	virtual ~AESConnectionLine();

    // GameObject
    void BeforeRender() override;

    void SetNodeTo(AESNode *nodeTo);
    void SetNodeFrom(AESNode *nodeFrom);

    bool HasFocus() const;
    bool IsMouseOver() const;

    AESNode *GetNodeTo() const;
    AESNode *GetNodeFrom() const;

private:
    bool m_hasFocus = false;
    UIImageRenderer *p_arrowImg = nullptr;
    UIContextMenu *p_contextMenu = nullptr;
    LineRenderer *p_lineRenderer = nullptr;

    AESNode *p_nodeTo   = nullptr;
    AESNode *p_nodeFrom = nullptr;

    void RemoveSelf();

    AESNode *GetFirstFoundNode() const;
    bool IsValidConnection(AESNode *oneNode, AESNode *otherNode) const;
    Vector3 GetConnectionPointLinePosition(AESNode *connPoint) const;
};

NAMESPACE_BANG_EDITOR_END

#endif // AESCONNECTIONLINE_H

