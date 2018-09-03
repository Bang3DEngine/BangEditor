#ifndef PROJECT_H
#define PROJECT_H

#include "Bang/GUID.h"
#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/Serializable.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class Project : public Serializable
{
    SERIALIZABLE(Project)

public:
    Project();
    virtual ~Project();

    Path GetProjectDirectory() const;
    Path GetProjectAssetsFilepath() const;
    Path GetProjectFilepath() const;
    String GetProjectName() const;
    const GUID& GetProjectRandomId() const;

    void SetProjectFilepath(const Path &projectFilepath);

    virtual bool OpenFirstFoundScene() const;
    Path GetFirstFoundScenePath() const;

    // Serializable
    virtual void ImportMeta(const MetaNode &metaNode) override;
    virtual void ExportMeta(MetaNode *metaNode) const override;

private:
    GUID m_id;
    Path m_projectFilepath;
};

NAMESPACE_BANG_EDITOR_END

#endif // PROJECT_H
