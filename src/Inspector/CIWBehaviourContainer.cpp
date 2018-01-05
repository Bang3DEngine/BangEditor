#include "BangEditor/CIWBehaviourContainer.h"

#include "Bang/UIButton.h"
#include "Bang/Extensions.h"
#include "Bang/UITextRenderer.h"

#include "BangEditor/EditorPaths.h"
#include "BangEditor/UIInputFile.h"
#include "BangEditor/BehaviourManager.h"
#include "BangEditor/BehaviourContainer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

CIWBehaviourContainer::CIWBehaviourContainer() {}
CIWBehaviourContainer::~CIWBehaviourContainer() {}

void CIWBehaviourContainer::Init()
{
    ComponentInspectorWidget::Init();

    SetName("CIWBehaviourContainer");
    SetTitle("Behaviour");

    p_sourceInputFile = GameObject::Create<UIInputFile>();
    p_sourceInputFile->SetExtensions(Extensions::GetSourceFileExtensions());
    p_sourceInputFile->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Source", p_sourceInputFile);

    SetLabelsWidth(60);
}

void CIWBehaviourContainer::UpdateValuesFromComponent()
{
    ComponentInspectorWidget::UpdateValuesFromComponent();

    p_sourceInputFile->SetPath( GetBehaviourContainer()->GetSourceFilepath() );
}

void CIWBehaviourContainer::OnValueChanged(Object*)
{
    GetBehaviourContainer()->SetSourceFilepath( p_sourceInputFile->GetPath() );
}

BehaviourContainer *CIWBehaviourContainer::GetBehaviourContainer() const
{
    return SCAST<BehaviourContainer*>( GetComponent() );
}
