/*
 * DOMImplementation.h
 *
 *  Created on: 17 Aug 2024
 *      Author: gavin
 */

#ifndef PARSERS_DOM_DOMIMPLEMENTATION_H
#define PARSERS_DOM_DOMIMPLEMENTATION_H

// Standard C++ library header files
#include <memory>
#include <optional>

namespace GCL::parsers::DOM
{
  class CDOMNodeDocType;
  class CDOMNodeDocument;

  class CDOMImplementation
  {
  public:
    std::unique_ptr<CDOMNodeDocType> createDocumentType(string_type const &qualifiedName, string_type const &publicID, string_type const &systemID);
    std::unique_ptr<CDOMNodeDocument> createHTMLDocument(std::optional<string_type> const &title);

    bool hasFeature() const noexcept { return true; }
  private:
    CDOMNodeDocument *parent;
  };
}

#endif /* PARSERS_DOM_DOMIMPLEMENTATION_H */
