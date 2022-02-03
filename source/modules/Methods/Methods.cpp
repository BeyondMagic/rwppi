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
 *
 */

#include <regex>
#include <string>
#include <iostream>
#include <future>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "tinyexpr/main.h"

#include "lexbor/css/css.h"
#include "lexbor/selectors/selectors.h"
#include "lexbor/html/interface.h"
#include "lexbor/html/html.h"
#include "lexbor/html/parser.h"

#include "Methods.hpp"

bool response_found = false;

static lxb_dom_node_t      * lexbor_body;
static lxb_html_document_t * lexbor_document;
static lxb_css_parser_t    * lexbor_parser;
static lxb_selectors_t     * lexbor_selectors;

#define PRINT_RESPONSE()       \
  if (!ctx.response.empty()) { \
    std::cout << ctx.response; \
    response_found = true;     \
  }

#define WALKER_FUNCTION(name)               \
  static lexbor_action_t                    \
  __##name(lxb_dom_node_t *node, void *ctx)

#define PRINT_FUNCTION(name)                                                       \
  lxb_status_t                                                                     \
  __##name (lxb_dom_node_t *node, lxb_css_selector_specificity_t *spec, void *ctx)

#define SELECTOR(name) \
  static const lxb_char_t s[] = name

#define METHOD(name) \
  context_t ctx;     \
  ctx.method = name

#define FIND(name)                                                                              \
  lxb_css_parser_t *copy_parser = lxb_css_parser_create();                                      \
  lxb_css_parser_init(copy_parser, NULL, NULL);                                                 \
  lxb_selectors_t *copy_selectors = lxb_selectors_create();                                     \
  lxb_selectors_init(copy_selectors);                                                           \
  lxb_dom_node_t *copy_body = lxb_dom_interface_node(lxb_html_document_body_element(lexbor_document)); \
  lxb_css_selector_list_t *list = lxb_css_selectors_parse(copy_parser, s, sizeof(s) - 1);       \
  lxb_status_t status = lxb_selectors_find(copy_selectors, copy_body, list, __##name , &ctx);   \
  lxb_css_selector_list_destroy_memory(list)

#define FAILURE(message)             \
  {                                    \
    std::cerr << message << std::endl; \
    exit(1);                           \
  }

// Format one single line of an element.
PRINT_FUNCTION(one_line)
{
  context_t *my = (context_t *) ctx;

  // 1. Save the text content of the element.
  const lxb_char_t * data = lxb_dom_node_text_content(node, nullptr);

  // 2. Print the text data of the element
  my->response = my->response + my->method + " " + (char *) data + "\n";

  return LXB_STATUS_OK;
};

// Format multiple lines of a single element.
PRINT_FUNCTION(multi_lines)
{
  context_t *my = (context_t *) ctx;

  // 1. Save the text content of the element.
  const lxb_char_t * data = lxb_dom_node_text_content(node, nullptr);;

  std::string page = (const char *) data;

  if (!page.empty()) {

    page = std::regex_replace(page, std::regex("\n"), "\n" + std::string(my->method) + " ");
    my->response = my->response + my->method + " " + page + "\n";

  }

  return LXB_STATUS_OK;
}

#include "Local.hpp"
#include "Google.hpp"

/*
 * Remote initializers of the method to gather the HTML content.
 *
 * It is using the blazing fast library called lexbor (which is in current development).
 */

MethodRemote::MethodRemote( const std::string & page )
{

  lxb_status_t status;

  lexbor_document = lxb_html_document_create();
  if (lexbor_document   == NULL   ) FAILURE("[Methods] Failed to create HTML Document");

  status = lxb_html_document_parse(lexbor_document, (lxb_char_t *) page.c_str(), page.length() - 1);
  if (status     != LXB_STATUS_OK ) FAILURE("[Methods] Failed to parse HTML");

  /* Create CSS parser. */
  lexbor_parser = lxb_css_parser_create();
  status = lxb_css_parser_init(lexbor_parser, NULL, NULL);
  if (status     != LXB_STATUS_OK ) FAILURE("[Methods] Failed to create CSS parser.");

  /* Selectors. */
  lexbor_selectors = lxb_selectors_create();
  status = lxb_selectors_init(lexbor_selectors);
  if (status     != LXB_STATUS_OK ) FAILURE("[Methods] ");

  /* Find DOM/HTML nodes by selectors. */
  lexbor_body = lxb_dom_interface_node(lxb_html_document_body_element(lexbor_document));
  if (lexbor_body       == NULL          ) FAILURE("[Methods] Failed to create BODY interface.");

}

void
MethodRemote::clean( void )
{

  /* Destroy Selectors object. */
  (void) lxb_selectors_destroy(lexbor_selectors, true);

  /* Destroy resources for CSS Parser. */
  (void) lxb_css_parser_destroy(lexbor_parser, true);

  /* Destroy HTML Document. */
  lxb_html_document_destroy(lexbor_document);

}
