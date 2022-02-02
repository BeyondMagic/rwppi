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

/*
 * Printers.
 */

PRINT_FUNCTION(GoogleLyrics)
{
  context_t *my = (context_t *) ctx;

  // A. Create string for the data handler.
  const lxb_char_t * data = lxb_dom_node_text_content(node, nullptr);
  std::string page = (const char *) data;

  // B. If there's something on the string (found something)
  if (!page.empty()) {

    page = std::regex_replace(page, std::regex("\n"), "\n" + my->method + " ");
    std::cout << my->method << " " << page << "\n";
    response_found = true;

  }

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

  SELECTOR(".PZPZlf > div[jsname=\"WbKHeb\"] > .ujudUb.xpdxpnd, .hwc > .BNeawe.tAd8D.AP7Wnd > div > .BNeawe.tAd8D.AP7Wnd");
  METHOD("GoogleLyrics");
  FIND(multi_lines);

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
