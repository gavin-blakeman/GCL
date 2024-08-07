#include "include/parsers/html/htmlNodeBase.h"

namespace GCL::parsers::html
{
  CHTMLNodeBase::child_pointer CHTMLNodeBase::insert(std::unique_ptr<CHTMLNodeBase> &&ins)
  {
    childCollection.push_back(std::move(ins));
  }
}

