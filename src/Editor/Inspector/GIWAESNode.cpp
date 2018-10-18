#include "BangEditor/GIWAESNode.h"

#include "Bang/Animation.h"
#include "Bang/AnimatorStateMachineNode.h"
#include "Bang/Assert.h"
#include "Bang/Color.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/Extensions.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/Path.h"
#include "Bang/ResourceHandle.h"
#include "Bang/Resources.h"
#include "Bang/Resources.tcc"
#include "Bang/String.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIInputText.h"
#include "Bang/UITextRenderer.h"
#include "BangEditor/AESNode.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/Inspector.h"
#include "BangEditor/UIInputFileWithPreview.h"

namespace Bang
{
class IEventsDestroy;
}

using namespace Bang;
using namespace BangEditor;

GIWAESNode::GIWAESNode()
{
}

GIWAESNode::~GIWAESNode()
{
}

void GIWAESNode::InitInnerWidgets()
{
    InspectorWidget::InitInnerWidgets();

    SetTitle("Animation State Node");
    SetName("GIWAESNode");

    GetInspectorWidgetTitle()
        ->GetEnabledCheckBox()
        ->GetGameObject()
        ->SetEnabled(false);
    GetInspectorWidgetTitle()->GetIcon()->SetImageTexture(
        EditorTextureFactory::GetAnimatorSMIcon());
    GetInspectorWidgetTitle()->GetIcon()->SetTint(Color::White);

    p_nameInput = GameObjectFactory::CreateUIInputText();
    p_nameInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_nodeAnimationInput = new UIInputFileWithPreview();
    p_nodeAnimationInput->SetExtensions({Extensions::GetAnimationExtension()});
    p_nodeAnimationInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    AddWidget("Name", p_nameInput->GetGameObject());
    AddWidget("Animation", p_nodeAnimationInput);

    SetLabelsWidth(100);
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
        p_nodeAnimationInput->SetPath(
            smNode->GetAnimation()
                ? smNode->GetAnimation()->GetResourceFilepath()
                : Path::Empty);
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
            smNode->SetAnimation(
                Resources::Load<Animation>(p_nodeAnimationInput->GetPath())
                    .Get());
        }
    }
}

void GIWAESNode::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    GameObject::OnDestroyed(object);

    ASSERT(object == p_aesNode);
    Inspector::GetActive()->Clear();
}
