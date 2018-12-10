#include "BangEditor/ObjectItem.h"

#include "Bang/Scene.h"
#include "Bang/Texture2D.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditorTextureFactory.h"

using namespace Bang;
using namespace BangEditor;

ObjectItem::ObjectItem()
{
    GetLabel()->GetText()->SetContent("None");
}

ObjectItem::~ObjectItem()
{
}

void ObjectItem::SetObject(Object *object)
{
    if (object != GetObject())
    {
        p_object = object;

        String labelText = "None";
        Texture2D *iconTex = nullptr;
        Color iconTint = Color::Zero();
        if (GetObject())
        {
            if (GameObject *go = DCAST<GameObject *>(GetObject()))
            {
                labelText = go->GetName();
                iconTex = EditorTextureFactory::GetWhiteTexture();
                iconTint = Color::White();
            }

            if (Component *comp = DCAST<Component *>(GetObject()))
            {
                labelText = comp->GetGameObject()->GetName() + "/" +
                            comp->GetClassName();
                iconTex = EditorTextureFactory::GetComponentIcon(comp);
                iconTint = EditorTextureFactory::GetComponentIconTint(comp);
            }
        }
        SetIcon(iconTex, iconTint);
        GetLabel()->GetText()->SetContent(labelText);
    }
}

Object *ObjectItem::GetObject() const
{
    return p_object;
}
