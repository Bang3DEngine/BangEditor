#ifndef UIINPUTARRAYROW_H
#define UIINPUTARRAYROW_H

#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

using namespace Bang;
namespace BangEditor
{
class UIInputArray;

class UIInputArrayRow : public GameObject
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(UIInputArrayRow);

public:
    UIInputArrayRow();
    virtual ~UIInputArrayRow() override;

    void Init(UIInputArray *inputArray);
    void SetContainedGameObject(GameObject *containedGo);

    GameObject *GetContainedGameObject() const;

private:
    GameObject *p_rowHLGo = nullptr;
    GameObject *p_containedGameObject = nullptr;
};
}

#endif  // UIINPUTARRAYROW_H
