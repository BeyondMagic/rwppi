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
bool is_atty = false;

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

std::string &
colour_method( std::string & method ) {

  if (is_atty)
    method = "\033[98;3m" + method + "\033[0m";

  return method;

}

std::string &
colour_type( std::string & type ) {

  if (is_atty)
    type = "\033[90;3m" + type + "\033[0m";

  return type;

}

void MethodRemote::extract_info ( std::string target,
                    std::string id,
                    lxb_status_t (*in)(lxb_dom_node_t *, lxb_css_selector_specificity_t *, void *),
                    lxb_html_document_t * document )
{

  // 1. To define the main (most-left forward) ID of the scraper.
  context_t data;
  data.method = id;
  data.i = 0;
  data.method = colour_method(data.method);

  // 2. To create a copy of the current parser already defined by the main selector.
  lxb_css_parser_t *copy_parser = lxb_css_parser_create();
  lxb_css_parser_init(copy_parser, NULL, NULL);

  // 3. 
  lxb_selectors_t *copy_selectors = lxb_selectors_create();
  lxb_selectors_init(copy_selectors);

  // 4. Most memory rampage will occur here since all HTML information of the document will be guarded here.
  //    For Google, this is a major problem since they pass-in JSON decrypted information for the client side to handle.
  lxb_dom_node_t *copy_body = lxb_dom_interface_node(lxb_html_document_body_element(document));

  // 5. 
  lxb_css_selector_list_t *list = lxb_css_selectors_parse(copy_parser, (const lxb_char_t *) target.c_str(), target.length());
  lxb_status_t status = lxb_selectors_find(copy_selectors, copy_body, list, in, &data);

  // 6. Clear off all of the selectors usage.
  lxb_css_selector_list_destroy_memory(list);

  // 7. 
  if (!data.response.empty()) {

    // A. Just print out the result once.
    std::cout << data.response;

    // B. To avoid double definition of the status of responses.
    if (!response_found) response_found = true;
  }

}
