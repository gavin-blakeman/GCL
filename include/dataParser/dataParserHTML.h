#ifndef DATAPARSER_DATAPARSERHTML_H
#define DATAPARSER_DATAPARSERHTML_H

// Standard C++ library
#include <filesystem>

// GCL Library
#include "include/dataParser/dataParserCore.h"

namespace GCL
{
  class CDataParserHTML
  {
    public:
      CDataParserHTML(CDataTokeniser::dataTables_t &, std::filesystem::path const &);
      ~CDataParserHTML() = default;

    private:
      CDataParserHTML() = delete;
      CDataParserHTML(CDataParserHTML const &) = delete;
      CDataParserHTML(CDataParserHTML &&) = delete;
      CDataParserHTML &operator=(CDataParserHTML const &) = delete;
      CDataParserHTML &operator=(CDataParserHTML &&) = delete;

      CDataTokeniser::dataTables_t &dataTables;
      std::filesystem::path fileName;

      void parseFile();
  };

}

#endif // DATAPARSER_DATAPARSERHTML_H
