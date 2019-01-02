#ifndef EDITORPROJECTMANAGER_H
#define EDITORPROJECTMANAGER_H

#include "Bang/Bang.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.h"
#include "Bang/ProjectManager.h"
#include "BangEditor/BangEditor.h"

using namespace Bang;
namespace BangEditor
{
class EditorProject;

class EditorProjectManager : public ProjectManager
{
public:
    EditorProjectManager();
    virtual ~EditorProjectManager() override;

    Project *OpenProject(const Path &projectFilepath) override;

    EditorProject *CreateNewProject(const Path &projectDirPath,
                                    const String &projectName);
    EditorProject *CreateNewProjectFileOnly(const Path &projectFilepath);
    void ExportProject(const EditorProject *project);
    void ExportCurrentProject();
    bool CloseCurrentProject() override;

    EditorProject *GetCurrentProject() const;
    static EditorProjectManager *GetInstance();

private:
    virtual Project *CreateNewProject() const override;
};
}

#endif  // EDITORPROJECTMANAGER_H
