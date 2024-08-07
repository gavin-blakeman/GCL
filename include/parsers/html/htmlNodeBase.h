#ifndef PARSERS_HTML_HTMLNODEBASE_H
#define PARSERS_HTML_HTMLNODEBASE_H

// Standard C++ library header files
#include <list>
#include <memory>

// Parsers header files
#include "include/parsers/html/htmlNodeTypes.h"

namespace GCL::parsers::html
{
  class CHTMLNodeBase
  {
  public:
    using child_type = CHTMLNodeBase;
    using child_collection = std::list<std::unique_ptr<child_type>>;
    using child_pointer = child_type *;
    using child_iterator = child_collection::iterator;
    using child_const_iterator = child_collection::const_iterator;

    CHTMLNodeBase(CHTMLNodeBase *parent) : parentNode(parent) {}
    virtual ~CHTMLNodeBase() = default;

    child_pointer insert(std::unique_ptr<CHTMLNodeBase> &&);

    child_iterator child_begin() { return childCollection.begin(); }
    child_const_iterator child_begin() const { return childCollection.cbegin(); }
    child_const_iterator child_cbegin() const { return childCollection.cbegin(); }

    child_iterator child_end() { return childCollection.end(); }
    child_const_iterator child_end() const { return childCollection.cend(); }
    child_const_iterator child_cend() const { return childCollection.cend(); }

    /*! @brief       Returns the type of the node.
     *  @returns     The type of the node.
     */
    virtual nodeType_t nodeType() const = 0;

    private:
      CHTMLNodeBase() = delete;
      CHTMLNodeBase(CHTMLNodeBase const &) = delete;
      CHTMLNodeBase(CHTMLNodeBase &&) = delete;
      CHTMLNodeBase &operator=(CHTMLNodeBase const &) = delete;
      CHTMLNodeBase &operator=(CHTMLNodeBase &&) = delete;

      CHTMLNodeBase *parentNode = nullptr;
      child_collection childCollection;


  };
}

#endif // PARSERS_HTML_HTMLNODEBASE_H
