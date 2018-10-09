#include "BangEditor/GIWAESNode.h"

#include "Bang/Extensions.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIInputText.h"
#include "Bang/UITextRenderer.h"
#include "Bang/TextureFactory.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/AnimatorStateMachine.h"
#include "Bang/AnimatorStateMachineNode.h"

#include "BangEditor/AESNode.h"
#include "BangEditor/Inspector.h"
#include "BangEditor/UIInputFile.h"
#include "BangEditor/EditorTextureFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

GIWAESNode::GIWAESNode()
{
}

GIWAESNode::~GIWAESNode()
{
}

void GIWAESNode::Init()
{
    InspectorWidget::Init();

    SetTitle("Animation State Node");
    SetName("GIWAESNode");

    GetInspectorWidgetTitle()->GetEnabledCheckBox()->GetGameObject()->
                               SetEnabled(false);
    GetInspectorWidgetTitle()->GetIcon()->SetImageTexture(
                               EditorTextureFactory::GetAnimatorSMIcon());
    GetInspectorWidgetTitle()->GetIcon()->SetTint(Color::White);

    p_nameInput = GameObjectFactory::CreateUIInputText();
    p_nameInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_nodeAnimationInput = GameObject::Create<UIInputFile>();
    p_nodeAnimationInput->SetExtensions(
                    {Extensions::GetAnimatorStateMachineExtension()} );
    p_nodeAnimationInput->EventEmitter<IEventsValueChanged>::
                          RegisterListener(this);

    AddWidget("Name", p_nameInput->GetGameObject());
    AddWidget("Animation", p_nodeAnimationInput);
    SetLabelsWidth(90);
}

void GIWAESNode::SetAESNode(AESNode *node)
{
    ASSERT(!p_aesNode);
    p_aesNode = node;
    p_aesNode->EventEmitter<IEventsDestroy>::RegisterListener(this);
}

AESNode *GIWAESNode::GetAESNode() const
{
    return p_aesNode;
}

void GIWAESNode::UpdateFromReference()
{
    InspectorWidget::UpdateFromReference();

    if (AnimatorStateMachineNode *smNode = GetAESNode()->GetSMNode())
    {
        p_nameInput->GetText()->SetContent(smNode->GetName());
    }
}

void GIWAESNode::OnValueChanged(EventEmitter<IEventsValueChanged> *ee)
{
    if (AnimatorStateMachineNode *smNode = GetAESNode()->GetSMNode())
    {
        if (ee == p_nameInput)
        {
            smNode->SetName(p_nameInput->GetText()->GetContent());
        }
        else if (ee == p_nodeAnimationInput)
        {
        }
    }
}

void GIWAESNode::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    ASSERT(object == p_aesNode);
    Inspector::GetActive()->Clear();
}
