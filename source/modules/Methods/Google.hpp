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

  RETURN_DEFAULT_WALKER();
}

PRINT_FUNCTION(GoogleWeather)
{
  context_t *my = (context_t *) ctx;

  // 1. The order of information given by Google.
  {

    switch (my->i) {
      case 0: my->type = "Celsius";    break;
      case 1: my->type = "Fahrenheit"; break;
      case 2: my->type = "Location";   break;
      case 3: my->type = "DayTime";    break;
      case 4: my->type = "Climate";    break;
      case 5: my->type = "Rain";       break;
      case 6: my->type = "Humidity";   break;
      case 7: my->type = "WindKM";     break;
      case 8: my->type = "WindMPH";    break;
    }

    my->type = colour_type(my->type);
  }

  RETURN_DEFAULT_WALKER();
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

  RETURN_DEFAULT_WALKER();
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

  RETURN_DEFAULT_WALKER();
}

/*
 * Selectors and finders.
 */

void MethodRemote::Google_LyricsInfo()
{

  // A. Title old version & new.
  // B. Singer old version & new.
  // C. Source old version & new.
  // D. Songwriters & Studios old version & new.
  // E. Album image showing.
  extract_info(
    ".kp-hc h2[data-attrid=\"title\"],"
    "div[data-attrid=\"title\"] > span[role=\"heading\"],"
    ".kp-hc a, div.wx62f[data-attrid=\"subtitle\"],"
    ".Oh5wg .j04ED,"
    ".Z1hOCe > div[data-lyricid] > div:not(:first-child),"
    ".xpdxpnd > .auw0zb,"
    "#tsuid21",
    "GoogleLyricsInformation",
    *__GoogleLyricsInformation);

}

void MethodRemote::Google_Math()
{

  // A. Result of expression shown.
  extract_info(
    ".vUGUtc[jsname=\"ubtiRe\"], #cwos",
    "GoogleMath",
    *__one_line);

}

void MethodRemote::Google_Lyrics()
{

  // I. New version of lyrics on many regions.
  extract_info(
    "div[data-lyricid] div[jsname=\"WbKHeb\"] > *",
    "GoogleLyrics",
    *__GoogleLyrics);

}

void MethodRemote::Google_Translation()
{

  // 1. From language.
  // 2. To language.
  // 3. From word.
  // 4. To word.
  // 5. From pronunciation.
  // 6. To pronunciation.
  extract_info(
    ".source-language,"
    ".target-language,"
    "#tw-source-text-ta,"
    "#tw-target-text,"
    "#tw-source-rmn,"
    "#tw-target-rmn",
    "GoogleTranslation",
    *__GoogleTranslation);

}

void MethodRemote::Google_UnitConversion()
{

  // 1. Header.
  // 2. From type.
  // 3. To type.
  // 4. From value.
  // 5. To value.
  // 6. Formula.
  extract_info(
    ".rYVYn.LNn04b option[selected],"
    "#ssSucf option[selected],"
    "#NotFQb option[selected],"
    "#HG5Seb input,"
    "#NotFQb input,"
    ".bjhkR",
    "GoogleUnitConversion",
    *__GoogleUnitConversion);

}

void MethodRemote::Google_InformationHeader()
{

  extract_info(
    ".g div[role=\"heading\"] > div > div",
    "GoogleInformationHeader",
    *__one_line);

}

void MethodRemote::Google_Correction()
{

  extract_info(
    "#fprs > *:nth-child(-n+2)",
    "GoogleCorrection",
    *__one_line);

}

void MethodRemote::Google_InformationTable()
{

  // A. One-liners of the table.
  extract_info(
    ".wp-ms.mnr-c div[data-attrid][style=\"clear:none\"]",
    "GoogleInformationTable",
    *__one_line);

}

void MethodRemote::Google_Weather()
{

  // 1. Celsius.
  // 2. Fahrenheit.
  // 3. Location.
  // 4. Date.
  // 5. Climate.
  // 6. Rain, Humidyt, Wind
  // 7. Image.

  // A. One-liners of the table.
  extract_info(
    "#wob_wc #wob_tm.wob_t,"
    "#wob_wc #wob_ttm.wob_t,"
    "#wob_wc #wob_loc,"
    "#wob_wc #wob_dts,"
    "#wob_wc #wob_dc,"
    "#wob_wc .wtsRwe > div:not(:last-child),"
    "#wob_wc .wtsRwe #wob_ws,"
    "#wob_wc .wtsRwe #wob_tws",
    "GoogleWeather",
    *__GoogleWeather);

}

void MethodRemote::Google_All()
{

  // A. Local methods call-in.
  auto math_handler              = std::async(&MethodRemote::Google_Math,              this);
  auto lyrics_handler            = std::async(&MethodRemote::Google_Lyrics,            this);
  auto lyricsinfo_handler        = std::async(&MethodRemote::Google_LyricsInfo,        this);
  auto correction_handler        = std::async(&MethodRemote::Google_Correction,        this);
  auto translation_handler       = std::async(&MethodRemote::Google_Translation,       this);
  auto unitconversion_handler    = std::async(&MethodRemote::Google_UnitConversion,    this);
  auto informationheader_handler = std::async(&MethodRemote::Google_InformationHeader, this);
  auto informationtable_handler  = std::async(&MethodRemote::Google_InformationTable,  this);
  auto weather_handler           = std::async(&MethodRemote::Google_Weather,           this);

  // B. Local methods call-out.
  math_handler.get();
  lyrics_handler.get();
  lyricsinfo_handler.get();
  correction_handler.get();
  translation_handler.get();
  unitconversion_handler.get();
  informationheader_handler.get();
  informationtable_handler.get();
  weather_handler.get();

}
