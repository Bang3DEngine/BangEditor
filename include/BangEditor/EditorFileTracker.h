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

class EditorFileTracker : public EventListener<IEventsFileTracker>,
                          public EventListener<IEventsProjectManager>,
                          public EventEmitter<IEventsFileTracker>
{
public:
	EditorFileTracker();
	virtual ~EditorFileTracker();

    void CheckFiles();
    void OnPathRenamed(const Path &previousPath, const Path &newPath);

    Time GetModificationTime(const Path &path) const;

    const USet<Path>& GetTrackedPaths() const;
    Array<Path> GetTrackedPathsWithExtensions(
                                        const Array<String> &extensions) const;
    Array<Path> GetTrackedPathsWithLastExtension(
                                        const Array<String> &extensions) const;

    static EditorFileTracker *GetInstance();

private:
    FileTracker *m_fileTracker = nullptr;

    void CheckForShaderModifications(const Path &modifiedPath);
    void CheckForBehaviourModifications(const Path &modifiedPath);
    FileTracker* GetFileTracker() const;

    // IFileTrackerListener
    void OnPathAdded(const Path &addedPath) override;
    void OnPathModified(const Path &modifiedPath) override;
    void OnPathRemoved(const Path &removedPath) override;

    // IEventsProjectManager
    void OnProjectOpen(const Project *project) override;
    void OnProjectClosed(const Project *project) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORFILETRACKER_H

