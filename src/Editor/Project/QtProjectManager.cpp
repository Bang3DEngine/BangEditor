#include "BangEditor/QtProjectManager.h"

#include "Bang/File.h"
#include "Bang/List.h"
#include "Bang/Array.h"
#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/SystemUtils.h"

#include "BangEditor/Project.h"
#include "BangEditor/EditorWindow.h"
#include "BangEditor/ProjectManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Path QtProjectManager::GetQtProjectDir()
{
    return Paths::GetProjectDir().Append("QtProject");
}

Path QtProjectManager::GetQtProjectFilepath()
{
    return QtProjectManager::GetQtProjectDir().Append(".creator");
}

bool QtProjectManager::IsQtCreatorOpenedCurrentProject()
{
    String output;
    SystemUtils::System("ps", {"-eo", "command"}, &output);

    bool alreadyOpened = false;
    Array<String> outputLines = output.Split<Array>('\n');
    for (const String &line : outputLines)
    {
        if (line.Contains("qtcreator") && !line.Contains("Bang.pro"))
        {
            alreadyOpened = true;
            break;
        }
    }
    return alreadyOpened;
}

void QtProjectManager::CreateQtProjectFile()
{
    Project *p_proj = ProjectManager::GetCurrentProject();
    Path projectDir = p_proj->GetProjectDirectory();
    const Path &engineIncludeDir = Paths::GetEngineIncludeDir();
    const Path &projAssetsDir = projectDir.Append("Assets");
    List<String> headers =
            projAssetsDir.GetFiles(true, {"h"}).To<List, String>();
    List<String> engineHeaders =
            engineIncludeDir.GetFiles(true, {"h"}).To<List, String>();
    List<String> sources =
            projAssetsDir.GetFiles(true, {"cpp"}).To<List, String>();
    List<String> engineSources =
            engineIncludeDir.GetFiles(true, {"cpp"}).To<List, String>();
    List<String> projIncPaths =
            projAssetsDir.GetSubDirectories(true).To<List, String>();

    List<Path> engineIncPaths = Paths::GetEngineIncludeDirs();
    List<String> engineIncPathsStr = engineIncPaths.To<List, String>();

    String headersString            = String::Join(headers,           "\n");
    String engineHeadersString      = String::Join(engineHeaders,     "\n");
    String sourcesString            = String::Join(sources,           "\n");
    String engineSourcesString      = String::Join(engineSources,     "\n");
    String projIncludePathsString   = String::Join(projIncPaths,      "\n");
    String engineIncludePathsString = String::Join(engineIncPathsStr, "\n");

    Path qtProjDir = QtProjectManager::GetQtProjectDir();
    File::CreateDirectory(qtProjDir);
    File::Write(qtProjDir.Append(".files"),
                headersString       + "\n" +
                sourcesString       + "\n" +
                engineHeadersString + "\n" +
                engineSourcesString);

    File::Write(qtProjDir.Append(".includes"),
                projIncludePathsString + "\n" +
                engineIncludePathsString);

    File::Write(qtProjDir.Append(".creator"), "[General]");
    File::Write(qtProjDir.Append(".config"), "");
}

void QtProjectManager::OpenBehaviourInQtCreator(const Path &behFilepath)
{
    bool alreadyOpened = QtProjectManager::IsQtCreatorOpenedCurrentProject();
    List<String> args = {};
    if (alreadyOpened) { args.PushBack("-client"); }
    //else
    {
        Path qtProjFilepath = QtProjectManager::GetQtProjectFilepath();
        args.PushBack(qtProjFilepath.GetAbsolute());
    }
    args.PushBack(behFilepath.GetAbsolute());

    bool ok = false;
    SystemUtils::System("qtcreator", args, nullptr, &ok);

    if (!ok)
    {
        SystemUtils::System("xdg-open", {behFilepath.GetAbsolute()});
    }
}

QtProjectManager::QtProjectManager()
{
}
