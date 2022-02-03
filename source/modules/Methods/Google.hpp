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

#include "lexbor/html/html.h"
#include "lexbor/dom/dom.h"
#include <iostream>

/*
 * Printers.
 */

WALKER_FUNCTION(GoogleLyricsWalker)
{
    lexbor_str_t * str;
    context_t *my = (context_t *) ctx;

    switch (lxb_dom_node_tag_id(node)) {
        case LXB_TAG__TEXT:
            str = &lxb_dom_interface_text(node)->char_data.data;
            my->response = my->response + my->method + " " + (char *)str->data;
            break;

        case LXB_TAG_BR:
            my->response = my->response + "\n";
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

PRINT_FUNCTION(GoogleLyrics)
{
  context_t *my = (context_t *) ctx;

  // A. Don't add a newline before any text.
  if (my->i != 0) my->response = my->response + my->method + "\n";

  lxb_dom_node_simple_walk(node, __GoogleLyricsWalker, my);

  // B. To separate the paragraphs of the lyrics.
  my->response = my->response + "\n";

  my->i++;

  return LXB_STATUS_OK;
}

PRINT_FUNCTION(GoogleLyricsInformation)
{
  context_t *my = (context_t *) ctx;

  // 1. Sub method to parse easier the given information.
  std::string method_type = {};

  // 2. The order of information given by Google.
  switch (my->i) {
    case 0: method_type = my->method + " Title "; break;
    case 1: method_type = my->method + " Singer "; break;
    case 2: method_type = my->method + " Source "; break;
    case 3: method_type = my->method + " Songwriters "; break;
    case 4: method_type = my->method + " Studios "; break;
  }

  // 3. Save the text content of the element.
  const lxb_char_t * data = lxb_dom_node_text_content(node, nullptr);

  // 4. Print the text data of the element
  my->response = my->response + method_type + (char *) data + "\n";

  my->i++;

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
  SELECTOR(".Ftghae > .SPZz6b > .qrShPb, .Ftghae > .SPZz6b > .wwUB2c, .uHNKed > .Oh5wg > .j04ED, .Oh5wg > .xpdxpnd > .auw0zb");

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

  // I. Version of lyrics on many regions.
  {
    SELECTOR(".PZPZlf > .bbVIQb[jsname=\"WbKHeb\"] > *");

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

void MethodRemote::Google_All( )
{

  // A. Local methods call-in.
  auto math_handler       = std::async(&MethodRemote::Google_Math,       this);
  auto lyrics_handler     = std::async(&MethodRemote::Google_Lyrics,     this);
  auto lyricsinfo_handler = std::async(&MethodRemote::Google_LyricsInfo, this);

  // B. Local methods call-out.
  math_handler.get();
  lyrics_handler.get();
  lyricsinfo_handler.get();

}
