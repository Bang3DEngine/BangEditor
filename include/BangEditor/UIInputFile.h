#ifndef UIINPUTFILE_H
#define UIINPUTFILE_H

#include "Bang/Path.h"
#include "BangEditor/UIInputFileOrObject.h"

namespace Bang
{
class IEventsDragDrop;
class IEventsValueChanged;
class UIButton;
class UIInputNumber;
class UIInputText;
}

using namespace Bang;
namespace BangEditor
{
class UIInputFile : public UIInputFileOrObject
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(UIInputFile);

public:
    UIInputFile();

    virtual void SetPath(const Path &path);
    void SetExtensions(const Array<String> &extensions);

    Path GetPath() const;
    const Array<String> &GetExtensions() const;
    bool HasExistingPath() const;
    AH<Texture2D> GetPreviewTextureFromPath(const Path &path);

protected:
    virtual ~UIInputFile() override;

    // UIInputFileOrObject
    bool CanDoZoom() const override;
    bool AcceptsDrag(
        EventEmitter<IEventsDragDrop> *dragDroppable) const override;
    void OnDropped(EventEmitter<IEventsDragDrop> *dragDroppable) override;
    void OnSearchButtonClicked() override;
    void OnOpenButtonClicked() override;

private:
    Path m_path = Path("undef");  // Set to empty in constructor
    Array<String> m_extensions;
};
}

#endif  // UIINPUTFILE_H
