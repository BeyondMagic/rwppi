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

#include "Methods.hpp"

/*
 * Walkers.
 */

WALKER_FUNCTION(GoogleLyricsWalker)
{
  lexbor_str_t * str;
  context_t *my = (context_t *) ctx;

  switch (lxb_dom_node_tag_id(node)) {
    case LXB_TAG__TEXT:
      str = &lxb_dom_interface_text(node)->char_data.data;
      my->response = my->response + my->method + ' ' + (char *) str->data;
      break;

    case LXB_TAG_BR:
      my->response = my->response + '\n';
      break;

    case LXB_TAG__EM_COMMENT:
    case LXB_TAG_SCRIPT:
    case LXB_TAG_STYLE:
      /* Skip node and his children's. */
      return LEXBOR_ACTION_NEXT;

    default:
      break;
  }

  return LEXBOR_ACTION_OK;
}

/*
 * Printers.
 */

PRINT_FUNCTION(GoogleLyrics)
{
  context_t *my = (context_t *) ctx;

  // A. Don't add a newline before any text.
  if (my->i != 0) my->response = my->response + my->method + '\n';

  lxb_dom_node_simple_walk(node, __GoogleLyricsWalker, my);

  // B. To separate the paragraphs of the lyrics.
  my->response = my->response + '\n';

  my->i++;

  return LXB_STATUS_OK;
}

PRINT_FUNCTION(GoogleLyricsInformation)
{
  context_t *my = (context_t *) ctx;

  // 1. The order of information given by Google.
  {
    switch (my->i) {
      case 0: my->type = "Title";       break;
      case 1: my->type = "Singer";      break;
      case 2: my->type = "Source";      break;
      case 3: my->type = "Songwriters"; break;
      case 4: my->type = "Studios";     break;
    }

    my->type = colour_type(my->type);
  }

  // 2. Save the text content of the element.
  const lxb_char_t * lxb_data = lxb_dom_node_text_content(node, nullptr);
  const std::string data = std::string( (char *) lxb_data );

  // 3. ...
  if (!data.empty()) {
    my->response = my->response + my->method + ' ' + my->type + ' ' + data + '\n';
  }

  my->i++;
  my->type.clear();

  return LXB_STATUS_OK;
}

PRINT_FUNCTION(GoogleTranslation)
{
  context_t *my = (context_t *) ctx;

  // 1. The order of information given by Google.
  {
    switch (my->i) {
      case 0: my->type = "FromLanguage";      break;
      case 1: my->type = "ToLanguage";        break;
      case 2: my->type = "FromWord";          break;
      case 3: my->type = "ToWord";            break;
      case 4: my->type = "FromPronunciation"; break;
      case 5: my->type = "ToPronunciation";   break;
    }

    my->type = colour_type(my->type);
  }

  // 2. Save the text content of the element.
  const lxb_char_t * lxb_data = lxb_dom_node_text_content(node, nullptr);
  const std::string data = std::string( (char *) lxb_data );

  // 3. Print the text data of the element
  if (!data.empty()) {
    my->response = my->response + my->method + ' ' + my->type + ' ' + data + '\n';
  }

  my->i++;
  my->type.clear();

  return LXB_STATUS_OK;
}

PRINT_FUNCTION(GoogleUnitConversion)
{
  context_t *my = (context_t *) ctx;

  // 2. The order of information given by Google.
  {
    const lxb_char_t name[] = "value";
    const lxb_char_t * value;
    std::string value_handler;
    size_t value_len;

    switch (my->i) {
      case 0:
        my->type = "Header";
        break;
      case 1:
        my->type = "FromType";
        break;
      case 2:
        my->type = "ToType";
        break;
      case 3:
        value = lxb_dom_element_get_attribute(lxb_dom_interface_element(node), name, 5, &value_len);
        my->type = "FromValue";
        value_handler = std::string( (char * ) value );
        break;
      case 4:
        value = lxb_dom_element_get_attribute(lxb_dom_interface_element(node), name, 5, &value_len);
        my->type = "ToValue";
        value_handler = std::string( (char * ) value );
        break;
      case 5:
        my->type = "Formula";
        break;
    }

    my->type = colour_type(my->type);

    if (!value_handler.empty())
      my->response = my->response + my->method + ' ' + my->type + ' ' + value_handler + '\n';

  }

  // 3. Save the text content of the element.
  const lxb_char_t * lxb_data = lxb_dom_node_text_content(node, nullptr);
  std::string data = std::string( (char *) lxb_data );

  // 4. Print the text data of the element
  if (!data.empty()) {
    my->response = my->response + my->method + ' ' + my->type + ' ' + data + '\n';
  }

  my->i++;

  my->type.clear();

  return LXB_STATUS_OK;
}

/*
 * Selectors and finders.
 */

void MethodRemote::Google_LyricsInfo()
{

  // A. Title.
  // B. Singer.
  // C. Source.
  // D. Songwriters & Studios.
  SELECTOR("div[data-attrid=\"title\"], div[data-attrid=\"subtitle\"], .f41I7.ai4HXb.j04ED, .f41I7.ai4HXb > .auw0zb");

  METHOD("GoogleLyricsInformation");
  FIND(GoogleLyricsInformation);

  PRINT_RESPONSE();

}

void MethodRemote::Google_Math()
{

  // A. Result of expression shown..
  SELECTOR("#cwos");

  METHOD("GoogleMath");
  FIND(one_line);
  PRINT_RESPONSE();

}

void MethodRemote::Google_Lyrics()
{

  // I. New version of lyrics on many regions.
  {
    SELECTOR("div[data-lyricid] div[jsname=\"WbKHeb\"] > *");

    METHOD("GoogleLyrics");
    FIND(GoogleLyrics);
    PRINT_RESPONSE();
  }

  // II. Version of lyrics on many other regions.
  {
    SELECTOR(".hwc > .BNeawe.tAd8D.AP7Wnd > div > .BNeawe.tAd8D.AP7Wnd");

    METHOD("GoogleLyrics");
    FIND(multi_lines);
    PRINT_RESPONSE();
  }

  // NOTE: No idea why Google has two different versions. Don't @ me.

}

void MethodRemote::Google_Translation()
{

  // 1. From language.
  // 2. To language.
  // 3. From word.
  // 4. To word.
  // 5. From pronunciation.
  // 6. To pronunciation.
  SELECTOR(".source-language, .target-language, #tw-source-text-ta, #tw-target-text, #tw-source-rmn, #tw-target-rmn");

  METHOD("GoogleTranslation");
  FIND(GoogleTranslation);

  PRINT_RESPONSE();

}

void MethodRemote::Google_UnitConversion()
{

  // 1. Header.
  // 2. From type.
  // 3. To type.
  // 4. From value.
  // 5. To value.
  // 6. Formula.
  SELECTOR(".rYVYn.LNn04b option[selected], #ssSucf option[selected], #NotFQb option[selected], #HG5Seb input, #NotFQb input, .bjhkR");

  METHOD("GoogleUnitConversion");
  FIND(GoogleUnitConversion);

  PRINT_RESPONSE();

}


void MethodRemote::Google_All()
{

  // A. Local methods call-in.
  auto math_handler           = std::async(&MethodRemote::Google_Math,           this);
  auto lyrics_handler         = std::async(&MethodRemote::Google_Lyrics,         this);
  auto lyricsinfo_handler     = std::async(&MethodRemote::Google_LyricsInfo,     this);
  auto translation_handler    = std::async(&MethodRemote::Google_Translation,    this);
  auto unitconversion_handler = std::async(&MethodRemote::Google_UnitConversion, this);

  // B. Local methods call-out.
  math_handler.get();
  lyrics_handler.get();
  lyricsinfo_handler.get();
  translation_handler.get();
  unitconversion_handler.get();

}
