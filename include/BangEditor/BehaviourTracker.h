#ifndef BEHAVIOURTRACKER_H
#define BEHAVIOURTRACKER_H

#include "Bang/Set.h"
#include "Bang/List.h"
#include "Bang/Path.h"
#include "Bang/FileTracker.h"

#include "BangEditor/ProjectManager.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class BehaviourTracker : public ProjectManagerListener,
                         public IFileTrackerListener
{
public:
	BehaviourTracker();
	virtual ~BehaviourTracker();

    void Update();
    void CheckForChanges();

    bool HasBeenModified(const Path &sourcePath) const;
    List<Path> GetIncludeDirs() const;

    const FileTracker& GetFileTracker() const;

    // ProjectManagerListener
    void OnProjectClosed(const Project *project) override;
    void OnProjectOpen(const Project *project) override;

    // IFileTrackerListener
    void OnPathAdded(const Path &addedPath) override;
    void OnPathModified(const Path &modifiedPath) override;
    void OnPathRemoved(const Path &removedPath) override;

private:
    FileTracker m_fileTracker;
    Set<Path> m_modifiedPathsFromLastUpdate;
};

NAMESPACE_BANG_EDITOR_END

#endif // BEHAVIOURTRACKER_H

