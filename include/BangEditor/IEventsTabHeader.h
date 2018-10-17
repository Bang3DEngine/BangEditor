#ifndef IEVENTSTABHEADER_H
#define IEVENTSTABHEADER_H

#include "Bang/IEvents.h"

#include "BangEditor/BangEditor.h"

namespace BangEditor
{
class UITabHeader;

class IEventsTabHeader
{
    IEVENTS_NS(IEventsTabHeader);

public:
    virtual void OnTabHeaderClicked(UITabHeader *header) = 0;
};
}

#endif  // IEVENTSTABHEADER_H
