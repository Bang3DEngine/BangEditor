#ifndef EDITORFILETRACKER_H
#define EDITORFILETRACKER_H

#include "Bang/Bang.h"
#include "Bang/FileTracker.h"
#include "Bang/EventListener.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/ProjectManager.h"

NAMESPACE_BANG_BEGIN
FORWARD class FileTracker;
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class BehaviourTracker;

class EditorFileTracker : public EventListener<IFileTrackerListener>,
                          public EventListener<IProjectManagerListener>
{
public:
	EditorFileTracker();
	virtual ~EditorFileTracker();

    FileTracker* GetFileTracker() const;
    BehaviourTracker* GetBehaviourTracker() const;

    void OnPathRenamed(const Path &previousPath, const Path &newPath);

    static EditorFileTracker *GetInstance();

private:
    FileTracker *m_fileTracker = nullptr;
    BehaviourTracker *m_behaviourFileTracker = nullptr;

    // IFileTrackerListener
    void OnPathAdded(const Path &addedPath) override;
    void OnPathModified(const Path &modifiedPath) override;
    void OnPathRemoved(const Path &removedPath) override;

    // IProjectManagerListener
    void OnProjectOpen(const Project *project) override;
    void OnProjectClosed(const Project *project) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORFILETRACKER_H

