#include "include/parsers/DOM/DOMNodeDocType.h"

namespace GCL::parsers::DOM
{

  CDOMNodeDocType::CDOMNodeDocType(CDOMNodeBase *parent,
                                   CDOMNodeDocType::string_type const &name,
                                   CDOMNodeDocType::string_type const &publicIdentifier,
                                   CDOMNodeDocType::string_type const &systemIdentifier)
    : CDOMNodeBase(parent), name_(name), publicIdentifier_(publicIdentifier), systemIdentifier_(systemIdentifier)
    {

    }
}  // namespace
