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

#include "Methods.hpp"
#include "Local.hpp"
#include "Google.hpp"

bool response_found = false;

// Format one single line of an element.
PRINT_FUNCTION(one_line)
{
  context_t *my = (context_t *) ctx;

  // 1. Save the text content of the element.
  const lxb_char_t * data = lxb_dom_node_text_content(node, nullptr);

  // 2. Print the text data of the element
  my->response = my->response + my->method + ' ' + (char *) data + '\n';

  return LXB_STATUS_OK;
};

// Format multiple lines of a single element.
PRINT_FUNCTION(multi_lines)
{
  context_t *my = (context_t *) ctx;

  // 1. Save the text content of the element.
  const lxb_char_t * data = lxb_dom_node_text_content(node, nullptr);

  std::string page = (const char *) data;

  if (!page.empty()) {

    page = std::regex_replace(page, std::regex("\n"), '\n' + std::string(my->method) + ' ');
    my->response = my->response + my->method + ' ' + page + '\n';

  }

  return LXB_STATUS_OK;
}

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
  if (status      != LXB_STATUS_OK ) FAILURE("[Methods] Failed to parse HTML");

  // 1. Create CSS parser.
  lexbor_parser = lxb_css_parser_create();
  status = lxb_css_parser_init(lexbor_parser, NULL, NULL);
  if (status      != LXB_STATUS_OK ) FAILURE("[Methods] Failed to create CSS parser.");

  // 2. Selectors.
  lexbor_selectors = lxb_selectors_create();
  status = lxb_selectors_init(lexbor_selectors);
  if (status      != LXB_STATUS_OK ) FAILURE("[Methods] ");

  // 3. Find DOM/HTML nodes by selectors.
  lexbor_body = lxb_dom_interface_node(lxb_html_document_body_element(lexbor_document));
  if (lexbor_body == NULL )          FAILURE("[Methods] Failed to create BODY interface.");

}

MethodRemote::~MethodRemote()
{

  // 1. Destroy Selectors object.
  (void) lxb_selectors_destroy(lexbor_selectors, true);

  // 2. Destroy resources for CSS Parser.
  (void) lxb_css_parser_destroy(lexbor_parser, true);

  // 3. Destroy HTML Document.
  lxb_html_document_destroy(lexbor_document);

}
