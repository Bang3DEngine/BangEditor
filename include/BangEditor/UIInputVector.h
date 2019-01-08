#ifndef UIINPUTVECTOR_H
#define UIINPUTVECTOR_H

#include <vector>

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class IEventsValueChanged;
class UIInputNumber;
class UILabel;
}

using namespace Bang;
namespace BangEditor
{
class UIInputVector : public GameObject,
                      public EventListener<IEventsValueChanged>,
                      public EventEmitter<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(UIInputVector);

public:
    UIInputVector();
    UIInputVector(int size);

    void SetSize(int size);

    void Set(const Vector2 &v);
    void Set(const Vector3 &v);
    void Set(const Vector4 &v);
    void Set(int i, float v);
    void SetBlocked(bool blocked);
    void SetStep(const Vector4 &step);
    void SetMinValue(const Vector4 &minValue);
    void SetMaxValue(const Vector4 &maxValue);
    void SetMinMaxValues(const Vector4 &minValue, const Vector4 &maxValue);

    float Get(int i) const;
    Vector2 GetVector2() const;
    Vector3 GetVector3() const;
    Vector4 GetVector4() const;
    Vector4 GetStep() const;

    int GetSize() const;

    const Array<UIInputNumber *> GetInputNumbers() const;

    bool HasFocus() const;

    void OnValueChanged(EventEmitter<IEventsValueChanged> *object) override;

protected:
    virtual ~UIInputVector() override;

private:
    int m_size = 4;
    Array<UIInputNumber *> m_inputNumbers;
};
}

#endif  // UIINPUTVECTOR_H
