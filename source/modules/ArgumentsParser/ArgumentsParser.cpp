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

#include "ArgumentsParser.hpp"

InputParser::InputParser (const int argc, char ** argv )
{
  // Loop through each argument and add it to the tokens private variable.
  for ( int i = 1; i < argc; ++i )
  {
    this->tokens.push_back( std::string(argv[i]) );
  }
}

const std::string
InputParser::get( const std::string & short_name, const std::string & long_name )
{
  std::vector <std::string>::const_iterator itr;

  GET_STRING(short_name)
  {
    const std::string parameter = *--itr;
    REMOVE_STRING(parameter);
    return parameter;
  }
  else
  {
    GET_STRING(long_name)
    {
      const std::string parameter = *--itr;
      REMOVE_STRING(parameter);
      return parameter;
    }
  }

  // Return nothing if it doesn't find.
  static const std::string empty_string("");
  return empty_string;
}

const std::string
InputParser::left ( void ) const {

  std::string s;

  // Add all the arguments from the vector into a string.
  for (std::string word : tokens)
  {
    s += " " + word;
  }

  // Find trailing characters from the left side and remove them.
  size_t start = s.find_first_not_of(" \n\r\t\f\v");
  s =  (start == std::string::npos) ? "" : s.substr(start);

  // Find trailing characters from the right side and remove them.
  size_t end = s.find_last_not_of(" \n\r\t\f\v");
  s = (end == std::string::npos) ? "" : s.substr(0, end + 1);

  return s;
}

bool
InputParser::exists( const std::string & short_name, const std::string & long_name )
{

  // Loop through the options and see if exists either short_name or long_name option.
  if ( EXIST_STRING(short_name) )
  {
    REMOVE_STRING(short_name);
    return true;
  }
  else if ( EXIST_STRING(long_name) )
  {
    REMOVE_STRING(long_name);
    return true;
  }

  return false;

}

#undef REMOVE_STRING
#undef GET_STRING
#undef EXIST_STRING
