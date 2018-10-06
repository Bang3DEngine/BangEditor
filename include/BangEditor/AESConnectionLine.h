#ifndef AESCONNECTIONLINE_H
#define AESCONNECTIONLINE_H

#include "Bang/Bang.h"
#include "Bang/GameObject.h"
#include "Bang/LineRenderer.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class AESConnectionPoint;

class AESConnectionLine : public GameObject
{
    GAMEOBJECT_EDITOR(AESConnectionLine);

public:
	AESConnectionLine();
	virtual ~AESConnectionLine();

    // GameObject
    void BeforeRender() override;

    void SetConnectionPointIn(AESConnectionPoint *connPointIn);
    void SetConnectionPointOut(AESConnectionPoint *connPointOut);

    AESConnectionPoint *GetConnectionPointIn() const;
    AESConnectionPoint *GetConnectionPointOut() const;
    AESConnectionPoint *GetFirstFoundConnectedPoint() const;

private:
    LineRenderer *p_lineRend = nullptr;

    AESConnectionPoint *p_connectionPointIn  = nullptr;
    AESConnectionPoint *p_connectionPointOut = nullptr;
    AESConnectionPoint *p_currentDragOtherConnectionPoint = nullptr;

    bool IsValidConnection(AESConnectionPoint *oneConnPoint,
                           AESConnectionPoint *otherConnPoint) const;
    Vector3 GetConnectionPointLinePosition(AESConnectionPoint *connPoint) const;
};

NAMESPACE_BANG_EDITOR_END

#endif // AESCONNECTIONLINE_H

