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

    if (my->i != 1) response_found = true;

    switch (lxb_dom_node_tag_id(node)) {
        case LXB_TAG__TEXT:
            str = &lxb_dom_interface_text(node)->char_data.data;
            std::cout << my->method << " " << (char *) str->data;
            break;

        case LXB_TAG_BR:
            std::cout << "\n";
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

  if (my->i != 0) std::cout << my->method + "\n";

  lxb_dom_node_simple_walk(node, __GoogleLyricsWalker, my);

  std::cout << "\n";

  my->i++;

  return LXB_STATUS_OK;
}

PRINT_FUNCTION(GoogleInformation)
{
  context_t *my = (context_t *) ctx;

  const lxb_char_t * data = lxb_dom_node_text_content(node, nullptr);

  std::string page = (const char *) data;

  if ( !page.empty() ) {

    page = std::regex_replace(page, std::regex("\n"), "\n" + std::string(my->method) + " ");
    std::cout << my->method << " " << page << "\n";
    response_found = true;

  }

  return LXB_STATUS_OK;
}


/*
 * Selectors and finders.
 */

void MethodRemote::Google_LyricsInfo()
{

  SELECTOR(".hwc > .BNeawe.uEec3.AP7Wnd");
  METHOD("GoogleInformation");
  FIND(GoogleInformation);

}

void MethodRemote::Google_Math()
{

  SELECTOR("#cwos");
  METHOD("GoogleMath");
  FIND(one_line);

}

void MethodRemote::Google_Lyrics()
{

  // I. Version of lyrics on many regions.
  {
    //SELECTOR(".PZPZlf > .bbVIQb[jsname=\"WbKHeb\"] > .ujudUb > *");
    SELECTOR(".PZPZlf > .bbVIQb[jsname=\"WbKHeb\"] > *");
    METHOD("GoogleLyrics");
    FIND(GoogleLyrics);
  }

  // II. Version of lyrics on many other regions.
  {
    SELECTOR(".hwc > .BNeawe.tAd8D.AP7Wnd > div > .BNeawe.tAd8D.AP7Wnd");
    METHOD("GoogleLyrics");
    FIND(multi_lines);
  }

  // NOTE: No idea why Google has two different versions. Don't @ me.

}

void MethodRemote::Google_All( )
{

  // Local methods call-in.
  auto math_handler       = std::async(&MethodRemote::Google_Math,       this);
  auto lyrics_handler     = std::async(&MethodRemote::Google_Lyrics,     this);
  auto lyricsinfo_handler = std::async(&MethodRemote::Google_LyricsInfo, this);

  // Local methods call-out.
  math_handler.get();
  lyrics_handler.get();
  lyricsinfo_handler.get();

}
