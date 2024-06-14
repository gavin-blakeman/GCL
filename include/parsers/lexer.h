#ifndef GCL_PARSERS_LEXER_H
#define GCL_PARSERS_LEXER_H

#include <atomic>
#include <istream>
#include <mutex>
#include <semaphore>
#include <shared_mutex>
#include <string>
#include <thread>
#include <vector>

#include <SCL>

#include "include/parsers/token.h"

namespace GCL::parsers
{
  class CLexer
  {
  public:
    /*! @brief      Constructor.
     *  @param[in]  is: The input stream to parse.
     */

    CLexer(std::istream &is);
    virtual ~CLexer() = default;

    std::vector<CToken> getTokens();

  protected:
    std::size_t lineNo = 0;
    std::size_t linePos = 0;
    std::size_t row = 0;
    std::size_t col = 0;
    std::vector<CToken> tokens;
    SCL::circularBuffer<char, 1024, false, false> buffer;

    /*! @brief      Checks if the next character in the stream matches the parameter.
     *  @param[in]  c: The character to test.
     *  @returns    true if the c matches the next character in the stream.
     */
    bool peek(char c, std::size_t);

    void consume(int n);
    bool match(std::string const &str);
    bool match(char const);

    virtual void next();

    /*! @brief    Fill the buffer when it falls below the minimum size.
     */
    virtual void fillBuffer();

    virtual void consume();

  private:
    CLexer() = delete;
    CLexer(CLexer const &) = delete;
    CLexer(CLexer &&) = delete;
    CLexer &operator=(CLexer const &) = delete;
    CLexer &operator=(CLexer &&) = delete;

    std::istream &inputStream;
    bool eos = false;
  };

} // namespace

#endif // GCL_PARSERS_LEXER_H
