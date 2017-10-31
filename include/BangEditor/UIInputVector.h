#ifndef UIINPUTVECTOR_H
#define UIINPUTVECTOR_H

#include "Bang/Array.h"
#include "Bang/GameObject.h"

#include "BangEditor/BangEditor.h"

NAMESPACE_BANG_BEGIN
FORWARD class UILabel;
FORWARD class UIInputNumber;
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UIInputVector : public GameObject
{
public:
	UIInputVector();
    UIInputVector(const String &labelText, int size);
	virtual ~UIInputVector();

    void SetLabelText(const String &text);
    void SetSize(int size);

    void Set(const Vector2 &v);
    void Set(const Vector3 &v);
    void Set(const Vector4 &v);
    void Set(int i, float v);

    Vector2 GetVector2() const;
    Vector3 GetVector3() const;
    Vector4 GetVector4() const;

private:
    Array<float> m_values;
    Array<UIInputNumber*> m_inputNumbers;

    UILabel *p_label = nullptr;

};

NAMESPACE_BANG_EDITOR_END

#endif // UIINPUTVECTOR_H

