#include "BangEditor/CIWBehaviourContainer.h"

#include "Bang/UIButton.h"
#include "Bang/Extensions.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIInputText.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UITextRenderer.h"
#include "Bang/BangPreprocessor.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/BehaviourContainer.h"

#include "BangEditor/EditorPaths.h"
#include "BangEditor/CIWBehaviour.h"
#include "BangEditor/UIInputColor.h"
#include "BangEditor/UIInputVector.h"
#include "BangEditor/EditorFileTracker.h"
#include "BangEditor/EditorBehaviourManager.h"
#include "BangEditor/UIInputFileWithPreview.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

CIWBehaviourContainer::CIWBehaviourContainer()
{
}

CIWBehaviourContainer::~CIWBehaviourContainer()
{
}

void CIWBehaviourContainer::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWBehaviourContainer");
    SetTitle("Behaviour");

    p_sourceInputFile = GameObject::Create<UIInputFileWithPreview>();
    p_sourceInputFile->SetExtensions(Extensions::GetSourceFileExtensions());
    p_sourceInputFile->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_sourceInputFile->SetZoomable(false);
    AddWidget("Source", p_sourceInputFile);
    AddWidget( GameObjectFactory::CreateUIHSeparator(), 10 );

    SetLabelsWidth(80);
}

void CIWBehaviourContainer::UpdateFromReflection(
                            const BPReflectedStruct &reflectStruct)
{
    // Clear
    for (GameObject *widget : m_ciwBehaviourHelper.GetWidgets())
    {
        RemoveWidget(widget);
    }

    // Add
    m_ciwBehaviourHelper.RecreateWidgetsFromReflection(reflectStruct, this);
    for (GameObject *widget : m_ciwBehaviourHelper.GetWidgets())
    {
        String name = m_ciwBehaviourHelper.GetWidgetToReflectedVar().
                      Get(widget).GetName();
        AddWidget(name, widget);
    }
    m_ciwBehaviourHelper.UpdateWidgetsFromMeta( GetBehaviourContainer()->
                                                GetInitializationMeta() );
    UpdateInitializationMetaFromWidgets();
}

void CIWBehaviourContainer::UpdateInitializationMetaFromWidgets()
{
    MetaNode initializationMeta = m_ciwBehaviourHelper.GetMetaFromWidgets();
    GetBehaviourContainer()->SetInitializationMeta( initializationMeta );
}

void CIWBehaviourContainer::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    Path srcPath = GetBehaviourContainer()->GetSourceFilepath();
    if (srcPath.IsFile())
    {
        Path headerPath = srcPath.WithExtension("h");
        Time::TimeT timeHeaderChanged = EditorFileTracker::GetInstance()->
                                        GetModificationTime(headerPath);
        if (timeHeaderChanged > m_prevTimeHeaderChanged)
        {
            Array<BPReflectedStruct> reflStructs =
                              BangPreprocessor::GetReflectStructs(headerPath);
            m_prevTimeHeaderChanged = timeHeaderChanged;
            if (reflStructs.Size() >= 1)
            {
                UpdateFromReflection(reflStructs.Front());
            }
        }

        SetTitle(srcPath.GetName());
        p_sourceInputFile->SetPath(srcPath);
    }
}

void CIWBehaviourContainer::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

    GetBehaviourContainer()->SetSourceFilepath( p_sourceInputFile->GetPath() );
    UpdateInitializationMetaFromWidgets();
}

BehaviourContainer *CIWBehaviourContainer::GetBehaviourContainer() const
{
    return SCAST<BehaviourContainer*>( GetComponent() );
}
