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

#pragma once

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

std::string &
colour_method( std::string & method);

std::string &
colour_type( std::string & type );

typedef struct {

  unsigned int i;
  std::string response;
  std::string method;
  std::string type;

} context_t;

class MethodRemote
{

  public:

    MethodRemote( const std::string & );

    ~MethodRemote();

    void
    extract_info(std::string, std::string,
                 lxb_status_t (*)(lxb_dom_node_t *, lxb_css_selector_specificity_t *, void *));

    void
    Google_All();

    void
    Google_Math();

    void
    Google_Lyrics();

    void
    Google_Correction();

    void
    Google_LyricsInfo();

    void
    Google_Translation();

    void
    Google_Definition();

    void
    Google_Weather();

    void
    Google_TrackList();

    void
    Google_InformationList();

    void
    Google_HeaderList();

    void
    Google_Chemistry();

    void
    Google_Pronunciation();

    void
    Google_UnitConversion();

    void
    Google_CurrencyConversion();

    void
    Google_InformationHeader();

    void
    Google_InformationWindow();

    void
    QuotesList();

    void
    TableSport();

    void
    InformationTable();

  private:
    lxb_dom_node_t      * lexbor_body;
    lxb_html_document_t * lexbor_document;
    lxb_css_parser_t    * lexbor_parser;
    lxb_selectors_t     * lexbor_selectors;

};

class MethodLocal
{

  public:

    MethodLocal( const std::string & );

    void
    Math();

    void
    All();

  private:
    std::string expression;

};

extern bool         response_found;
extern bool         is_atty;
extern lxb_status_t __one_line    (lxb_dom_node_t *node, lxb_css_selector_specificity_t *spec, void *ctx);
extern lxb_status_t __multi_lines (lxb_dom_node_t *node, lxb_css_selector_specificity_t *spec, void *ctx);

#define WALKER_FUNCTION(name)               \
  lexbor_action_t                    \
  __##name(lxb_dom_node_t *node, void *ctx)

#define PRINT_FUNCTION(name)                                                       \
  lxb_status_t                                                                     \
  __##name (lxb_dom_node_t *node, lxb_css_selector_specificity_t *spec, void *ctx)

#define FAILURE(message)               \
  {                                    \
    std::cerr << message << std::endl; \
    exit(1);                           \
  }
