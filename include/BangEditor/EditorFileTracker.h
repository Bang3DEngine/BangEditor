#ifndef EDITORFILETRACKER_H
#define EDITORFILETRACKER_H

#include <vector>

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/FileTracker.h"
#include "Bang/IEventsFileTracker.h"
#include "Bang/IEventsProjectManager.h"
#include "Bang/Time.h"
#include "Bang/USet.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class FileTracker;
class Path;
class String;
class Project;
}

using namespace Bang;
namespace BangEditor
{
class EditorFileTracker : public EventListener<IEventsFileTracker>,
                          public EventListener<IEventsProjectManager>,
                          public EventEmitter<IEventsFileTracker>
{
public:
    EditorFileTracker();
    virtual ~EditorFileTracker() override;

    void CheckFiles();
    void OnPathRenamed(const Path &previousPath, const Path &newPath);

    Time GetModificationTime(const Path &path) const;

    const USet<Path> &GetTrackedPaths() const;
    Array<Path> GetTrackedPathsWithExtensions(
        const Array<String> &extensions) const;
    Array<Path> GetTrackedPathsWithLastExtension(
        const Array<String> &extensions) const;

    static EditorFileTracker *GetInstance();

private:
    FileTracker *m_fileTracker = nullptr;

    void CheckForShaderModifications(const Path &modifiedPath);
    void CheckForBehaviourModifications(const Path &modifiedPath);
    FileTracker *GetFileTracker() const;

    // IFileTrackerListener
    void OnPathAdded(const Path &addedPath) override;
    void OnPathModified(const Path &modifiedPath) override;
    void OnPathRemoved(const Path &removedPath) override;

    // IEventsProjectManager
    void OnProjectOpen(const Project *project) override;
    void OnProjectClosed(const Project *project) override;
};
}

#endif  // EDITORFILETRACKER_H
