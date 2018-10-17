#ifndef QTPROJECTMANAGER_H
#define QTPROJECTMANAGER_H

#include "Bang/BangDefines.h"
#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/ThreadPool.h"
#include "BangEditor/BangEditor.h"

using namespace Bang;
namespace BangEditor
{
class QtProjectManager
{
public:
    static Path GetQtProjectFilepath();

    static void CreateQtProjectFile();
    static void OpenBehaviourInQtCreator(const Path &behaviourFilepath);

private:
    QtProjectManager();

    static Path GetQtProjectDir();
    static bool IsQtCreatorOpenedCurrentProject();
};
}

#endif  // QTPROJECTMANAGER_H
