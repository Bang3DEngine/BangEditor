#ifndef EDITORPROJECT_H
#define EDITORPROJECT_H

#include "Bang/Bang.h"
#include "Bang/Project.h"
#include "BangEditor/BangEditor.h"

using namespace Bang;
namespace BangEditor
{
class EditorProject : public Project
{
public:
    EditorProject();
    virtual ~EditorProject() override;

    void SetLastOpenScenePath(const Path &lastOpenScenePath);

    const Path &GetLastOpenScenePath() const;

    // Project
    bool OpenInitialScene() const override;

    // Serializable
    virtual void ImportMeta(const MetaNode &metaNode) override;
    virtual void ExportMeta(MetaNode *metaNode) const override;

private:
    Path m_lastOpenScenePath = Path::Empty();
};
}

#endif  // EDITORPROJECT_H
