/*
 * Copyright 2021-2022 João F. BeyondMagic <koetemagie@gmail.com>
 *
 *Licensed under the Apache License, Version 2.0 (the "License");
 *you may not use this file except in compliance with the License.
 *You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *Unless required by applicable law or agreed to in writing, software
 *distributed under the License is distributed on an "AS IS" BASIS,
 *WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *See the License for the specific language governing permissions and
 *limitations under the License.
 */

#include <string>
#include <iostream>
#include <future>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>

#include "tinyexpr/main.h"

#include "lexbor/css/css.h"
#include "lexbor/selectors/selectors.h"
#include "lexbor/html/interface.h"
#include "lexbor/html/html.h"
#include "lexbor/html/parser.h"

bool response_found = false;

/*
 * Inneficient way to do this.
 *
 * Could be replaced, yet to see a response from https://github.com/lexbor/lexbor/issues/151.
 */

#define PRINT_ML(name) \
  lxb_status_t                                                                    \
  _##name (lxb_dom_node_t *node, lxb_css_selector_specificity_t *spec, void *ctx) \
  {                                                                               \
    const lxb_char_t * data = lxb_dom_node_text_content(node, nullptr);           \
    std::string page = (const char *) data;                                       \
    page = std::regex_replace(page, std::regex("\n"), "\n" + std::string(#name) + " "); \
    if (!page.empty()){                                                           \
      std::cout << #name << " " << page << "\n";                                  \
      response_found = true;                                                      \
    }                                                                             \
    return LXB_STATUS_OK;                                                         \
  }

#define PRINT_OL(name)                                                            \
  lxb_status_t                                                                    \
  _##name (lxb_dom_node_t *node, lxb_css_selector_specificity_t *spec, void *ctx) \
  {                                                                               \
    const lxb_char_t * data = lxb_dom_node_text_content(node, nullptr);           \
    std::string page = (const char *) data;                                       \
    if (!page.empty()){                                                           \
      std::cout << #name << " " << page << "\n";                                  \
      response_found = true;                                                      \
    }                                                                             \
    return LXB_STATUS_OK;                                                         \
  }

#define FIND(name)                                                                              \
  lxb_css_parser_t *copy_parser = lxb_css_parser_create();                                      \
  lxb_css_parser_init(copy_parser, NULL, NULL);                                                 \
  lxb_selectors_t *copy_selectors = lxb_selectors_create();                                     \
  lxb_selectors_init(copy_selectors);                                                           \
  lxb_dom_node_t *copy_body = lxb_dom_interface_node(lxb_html_document_body_element(document)); \
  lxb_css_selector_list_t *list = lxb_css_selectors_parse(copy_parser, s, sizeof(s) - 1);       \
  lxb_status_t status = lxb_selectors_find(copy_selectors, copy_body, list, _##name , NULL);    \
  lxb_css_selector_list_destroy_memory(list);

#define FAILURE(MESSAGE)                                           \
  {                                                                \
    std::cerr << MESSAGE << std::endl;                             \
    exit(1);                                                       \
  }

/*
 * Anything that can be computed locally needs to be put in here, all of those functions will run not matter the query.
 * So be careful with the speed of it.
 */
struct MethodLocal
{

  /*
   * Return the value of an expression, it will returns the first value it encounters.
   * It will simply return NaN for invalid expressions.
   */
  void
  math( const std::string & );

  /*
   * Run all functions initialised above.
   */
  const bool
  all( const std::string & );

};

/*
 * Scrapers for a web-page.
 */
class MethodRemote
{

  private:
    lxb_dom_node_t *body;
    lxb_html_document_t *document;
    lxb_css_parser_t *parser;
    lxb_selectors_t *selectors;

  public:

    //

     /*
     * Follow the order of Util::print_methods as listed in there.
     */

    void
    google_math();

    void
    google_lyrics();

    void
    google_lyricsinfo();

    void
    google_definition();

    void
    google_translation();

    // Weather            -> current weather
    // Tracklist          -> noisia outer edges tracklist
    // InformationList    -> social network cast
    // HeaderList         -> Need for Speed Heat cars list
    // Chemistry          -> density of hydrogen
    // Pronunciation      -> pronounce linux
    // UnitConversion     -> 1m into 1cm
    // CurrencyConversion -> 1 USD in rupee
    // InformationHeader  -> christmas day
    // InformationWindow  -> who is garfield
    // QuotesList         -> mahatma gandhi quotes
    // TableSport         -> Chelsea next game
    // InformationTable   -> the office

    /* Initialise lexbor. */
    MethodRemote( const std::string &);

    /* Release resources. */
    void
    clean( void );

    /*
     * Compiled methods for certain pages.
     */
    const bool
    google();

};

/*
 * Local
 */

void
MethodLocal::math( const std::string & expression )
{

  const double answer = te_interp(expression.c_str(), 0);

  if (!std::isnan(answer)) std::cout << answer << " Math " << " Local ";

}

/*
 *
 *
 * Compiled.
 *
 *
 */

const bool
MethodLocal::all( const std::string & expression )
{

  // Local methods call-in.
  auto math_handler = std::async(&MethodLocal::math, this, expression);

  // Local methods call-out.
  math_handler.get();

  return response_found;

}

/*
 *
 *
 *
 *
 * Remote.
 *
 *
 *
 */

MethodRemote::MethodRemote( const std::string & page )
{

  lxb_status_t status;

  document = lxb_html_document_create();
  if (document   == NULL          ) FAILURE("[Methods] Failed to create HTML Document");

  status = lxb_html_document_parse(document, (lxb_char_t *) page.c_str(), page.length() - 1);
  if (status     != LXB_STATUS_OK ) FAILURE("[Methods] Failed to parse HTML");

  /* Create CSS parser. */
  parser = lxb_css_parser_create();
  status = lxb_css_parser_init(parser, NULL, NULL);
  if (status     != LXB_STATUS_OK ) FAILURE("[Methods] ");

  /* Selectors. */
  selectors = lxb_selectors_create();
  status = lxb_selectors_init(selectors);
  if (status     != LXB_STATUS_OK ) FAILURE("[Methods] ");

  /* Find DOM/HTML nodes by selectors. */
  body = lxb_dom_interface_node(lxb_html_document_body_element(document));
  if (body       == NULL          ) FAILURE("[Methods] ");

}

void
MethodRemote::clean( void )
{

  /* Destroy Selectors object. */
  (void) lxb_selectors_destroy(selectors, true);

  /* Destroy resources for CSS Parser. */
  (void) lxb_css_parser_destroy(parser, true);

  /* Destroy HTML Document. */
  lxb_html_document_destroy(document);

}

/*
 *
 *
 *
 *
 */

/*
 *
 *
 * Google.
 *
 *  
 */

PRINT_ML(GoogleLyrics) // TODO: PRINT_ML
void MethodRemote::google_lyrics()
{

  static const lxb_char_t s[] = ".hwc > .BNeawe.tAd8D.AP7Wnd > div > .BNeawe.tAd8D.AP7Wnd";
  FIND(GoogleLyrics)

}

PRINT_OL(GoogleMath)
void MethodRemote::google_math()
{

  static const lxb_char_t s[] = "#cwos";
  FIND(GoogleMath)

}

/*
 *
 *
 *
 *
 *
 *
 * Compiled.
 *
 *
 *
 *
 *
 *
 */

const bool
MethodRemote::google( )
{

  // Local methods call-in.
  auto math_handler   = std::async(&MethodRemote::google_math,   this);
  auto lyrics_handler = std::async(&MethodRemote::google_lyrics, this);
  //auto lyricsinfo_handler = std::async(&MethodRemote::google_lyricsinfo, this);

  // Local methods call-out.
  math_handler.get();
  lyrics_handler.get();
  //lyricsinfo_handler.get();

  return response_found;

}

#undef PRINT
#undef RESPONSE
