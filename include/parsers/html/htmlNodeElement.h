//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers::HTML Parser
// FILE:                htmlElements.h
// LANGUAGE:            C++
// TARGET OS:           None.
// NAMESPACE:           GCL
// AUTHOR:              Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2024 Gavin Blakeman.
//                      This file is part of the General Class Library (GCL)
//
//                      GCL is free software: you can redistribute it and/or modify it under the terms of the GNU General
//                      Public License as published by the Free Software Foundation, either version 2 of the License, or
//                      (at your option) any later version.
//
//                      GCL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
//                      implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
//                      for more details.
//
//                      You should have received a copy of the GNU General Public License along with GCL.  If not,
//                      see <http://www.gnu.org/licenses/>.
//
// OVERVIEW:            Class that represents the HTML elements.
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-06-18 GGB - File Created
//
//**********************************************************************************************************************************

#ifndef PARSERS_HTML_HTMLELEMENT_H
#define PARSERS_HTML_HTMLELEMENT_H

// Standard C++ library header files
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <variant>

// GCL header files
#include "include/parsers/html/htmlAttributes.h"
#include "include/parsers/html/htmlNodeBase.h"

/* An element corresponds to a tag.
 * An elements can have a list of attributes. The attributes apply to this element (and possible elements below it in the
 * structure.)
 * An element can also have child elements.
 */

namespace GCL::parsers::html
{
  enum htmlElements_e
  {
    HTML_UNKNOWN,

    HTML_A,             // 4.5.1
    HTML_ABBR,          // 4.5.9
    HTML_ADDRESS,       // 4.3.10
    HTML_AREA,          // 4.8.13
    HTML_ARTICLE,       // 4.3.2
    HTML_ASIDE,         // 4.3.5
    HTML_AUDIO,         // 4.8.9

    HTML_B,             // 4.5.21
    HTML_BASE,          // 4.3.4
    HTML_BDI,           // 4.5.24
    HTML_BDO,           // 4.5.25
    HTML_BLOCKQUOTE,    // 4.4.4
    HTML_BODY,          // 4.3.1
    HTML_BR,            // 4.5.27

    HTML_CAPTION,       // 4.9.2
    HTML_CITE,          // 4.5.6
    HTML_CODE,          // 4.5.15
    HTML_COL,           // 4.9.4
    HTML_COLGROUP,      // 4.9.3

    HTML_DATA,          // 4.5.13
    HTML_DD,            // 4.4.11
    HTML_DEL,           // 4.7.2
    HTML_DFN,           // 4.5.8
    HTML_DIV,           // 4.4.16
    HTML_DL,            // 4.4.9
    HTML_DT,            // 4.4.10

    HTML_EM,            // 4.5.2
    HTML_EMBED,         // 4.8.6

    HTML_FIGCAPTION,    // 4.4.13
    HTML_FIGURE,        // 4.4.12
    HTML_FOOTER,        // 4.3.9

    HTML_H1,            // 4.3.6
    HTML_H2,            // 4.3.6
    HTML_H3,            // 4.3.6
    HTML_H4,            // 4.3.6
    HTML_H5,            // 4.3.6
    HTML_H6,            // 4.3.6
    HTML_HEAD,          // 4.2.1
    HTML_HEADER,        // 4.3.8
    HTML_HGROUP,        // 4.3.7
    HTML_HR,            // 4.4.2
    HTML_HTML,

    HTML_I,             // 4.5.20
    HTML_IFRAME,        // 4.8.5
    HTML_IMG,           // 4.8.3
    HTML_INPUT,
    HTML_INS,           // 4.7.1

    HTML_KBD,           // 4.5.18

    HTML_LI,            // 4.4.8
    HTML_LINK,          // 4.2.4

    HTML_MAIN,          // 4.4.14
    HTML_MAP,           // 4.8.12
    HTML_MARK,          // 4.5.23
    HTML_MATH,          // 4.8.15
    HTML_MEDIA,         // 4.8.11
    HTML_MENU,          // 4.4.7
    HTML_META,          // 4.2.5

    HTML_NAV,           // 4.3.4
    HTML_NOBR,          // Obsolete. Not to be used when writing documents.

    HTML_OBJECT,        // 4.8.7
    HTML_OL,            // 4.4.5

    HTML_P,             // 4.4.1
    HTML_PICTURE,       // 4.8.1
    HTML_PRE,           // 4.4.3

    HTML_Q,             // 4.5.7

    HTML_RP,            // 4.5.12
    HTML_RT,            // 4.5.11
    HTML_RUBY,          // 4.5.10

    HTML_S,             // 4.5.5
    HTML_SAMP,          // 4.5.17
    HTML_SEARCH,        // 4.4.15
    HTML_SECTION,       // 4.3.3
    HTML_SCRIPT,
    HTML_SMALL,         // 4.5.4
    HTML_SOURCE,        // 4.8.2
    HTML_SPAN,          // 4.5.26
    HTML_STRONG,        // 4.5.3
    HTML_STYLE,         // 4.2.6
    HTML_SUB,           // 4.5.19
    HTML_SUP,           // 4.5.19
    HTML_SVG,           // 4.8.16

    HTML_TABLE,         // 4.9.1
    HTML_TBODY,         // 4.9.5
    HTML_TFOOT,         // 4.9.7
    HTML_THEAD,         // 4.9.6
    HTML_TD,            // 4.5.9
    HTML_TEMPLATE,
    HTML_TEXTAREA,
    HTML_TH,            // 4.9.10
    HTML_TIME,          // 4.5.14
    HTML_TITLE,         // 4.2.2
    HTML_TR,            // 4.5.8
    HTML_TRACK,         // 4.8.10

    HTML_U,             // 4.5.22
    HTML_UL,            // 4.4.6

    HTML_VAR,           // 4.5.16
    HTML_VIDEO,         // 4.8.8

    HTML_WBR,           // 4.5.28
  };

  class CHTMLElement : public CHTMLNodeBase
  {
  public:
    enum categories_e
    {
      CAT_NONE,
      CAT_METADATA,
      CAT_FLOWCONTROL,
      CAT_SECTIONING,
      CAT_HEADING,
      CAT_PHRASING,
      CAT_EMBEDDED,
      CAT_INTERACTIVE,
    };
    using attribute_type = CHTMLAttribute;
    using attribute_collection = std::map<std::string, attribute_type>;
    using attribute_iterator = attribute_collection::iterator;
    using attribute_const_iterator = attribute_collection::const_iterator;
    using attribute_reference = attribute_type &;
    using attribute_const_reference = attribute_type const &;
    using attribute_pointer = attribute_type *;
    using child_type = CHTMLElement;
    using child_collection = std::list<std::unique_ptr<CHTMLNodeBase>>;
    using child_pointer = child_type *;
    using child_vector = std::vector<child_pointer>;
    using child_reference = child_type &;
    using child_const_reference = child_type const &;
    using child_iterator = child_collection::iterator;
    using child_const_iterator = child_collection::const_iterator;
    using child_ref = std::reference_wrapper<child_type>;
    using child_byType = std::map<htmlElements_e, child_ref>;

    CHTMLElement(CHTMLNodeBase *parent, std::string const &);
    ~CHTMLElement() = default;

    /*! @brief      Returns true if the node as attributes.
     *  @returns    true if the node has attributes.
     *  @throws     noexcept
     */
    bool hasAttributes() const noexcept { return !attributes.empty(); }

    std::string type() const noexcept;
    std::string value() const noexcept { return elementValue; }
    void value(std::string const &) noexcept;

    void insert(CHTMLAttribute &&);
    void insert(std::string const &attr, std::string const &val);

    child_pointer insert(std::unique_ptr<CHTMLNodeBase> &&);
    child_pointer insert(CHTMLNodeBase *, std::string const &);

    attribute_iterator attributes_begin() { return attributes.begin(); }
    attribute_const_iterator attributes_begin() const { return attributes.cbegin(); }
    attribute_const_iterator attributes_cbegin() const { return attributes.cbegin(); }

    attribute_iterator attributes_end() { return attributes.end(); }
    attribute_const_iterator attributes_end() const { return attributes.cend(); }
    attribute_const_iterator attributes_cend() const { return attributes.cend(); }

    attribute_const_reference operator[](std::string const &) const;

    std::string title();
    std::string lang();
    std::string translate();
    std::string dir();

    bool hidden();

    child_iterator child_begin() { return elementChildren.begin(); }
    child_const_iterator child_begin() const { return elementChildren.cbegin(); }
    child_const_iterator child_cbegin() const { return elementChildren.cbegin(); }

    child_iterator child_end() { return elementChildren.end(); }
    child_const_iterator child_end() const { return elementChildren.cend(); }
    child_const_iterator child_cend() const { return elementChildren.cend(); }


    /*! @brief      Determines if the specified element is a void element. (open/close at the same time.)
     *  @param[in]  element: The element text to check.
     *  @returns    true if the element is a void element.
     */
    static bool isVoid(std::string const &element) noexcept;

    /*! @brief      Determines if the specified element is a void element. (open/close at the same time.)
     *  @param[in]  element: The element text to check.
     *  @returns    true if the element is a void element.
     */
    static bool isVoid(htmlElements_e element) noexcept;

    /*! @brief      Returns the string associated with an element type.
     *  @param[in]  elementName: The string name of the element.
     *  @returns    The element Type.
     *  @throws     noexcept
     */
    static htmlElements_e string2elementType(std::string const &) noexcept;

    /*! @brief      Converts an elementType to a string.
     *  @param[in]  type: The type to convert.
     *  @returns    The string represenation of the type.
     *  @throws     noexcept
     */
    static std::string elementType2string(htmlElements_e type) noexcept;

  private:
    CHTMLElement() = delete;

    using elementType_t = std::variant<std::monostate, htmlElements_e, std::string>;


    elementType_t elementType;
    std::string elementValue;
    attribute_collection attributes;
    child_collection elementChildren;

    friend bool operator==(CHTMLElement const &, CHTMLElement const &) { return false; };
    friend bool operator==(CHTMLElement const &, std::string const &) { return false; };
    friend bool operator==(CHTMLElement const &, htmlElements_e) { return false; };
  };

  bool operator==(std::string const &, htmlElements_e);
  bool operator!=(std::string const &str, htmlElements_e et);

} // namespace

#endif // PARSERS_HTML_HTMLELEMENT_H
