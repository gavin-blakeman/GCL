#include "include/dataParser/dataParserHTML.h"

// Standard C++ library header files.
#include <fstream>

// GCL header filesystem
#include "include/parsers/DOM/DOMNodeDocument.h"
#include "include/error.h"

namespace GCL
{
  CDataParserHTML::CDataParserHTML(CDataTokeniser::dataTables_t &dt, std::filesystem::path const &fn)
    : dataTables(dt), fileName(fn)
  {
    parseFile();
  }

  void CDataParserHTML::parseFile()
  {
    std::ifstream ifs;

    ifs.open(fileName);
    if (!ifs.good())
    {
      CODE_ERROR();
      // Does not return.
    }

    //parsers::html::CHTMLDocument DOM(ifs);


    //parsers::html::CHTMLDocument::const_iterator iter = DOM.find("table", DOM.cbegin());

//    while (iter != DOM.cend())
//    {
//      dataTables.push_back({});
//      parsers::html::CHTMLElement::child_const_iterator childIter = iter->child_begin();
//
//      // Drop everything on the way to the first <tr>
//
////      while (chldIter->type() != "tr") {++iter;}
////
////      while (iter->type() == "tr")
////      {
////        ++iter;
////        while (iter->type() == "td")
////        {
////          dataTables.back().second = iter->value();
////          ++iter;
////        }
////      }
//    }
  }
}
