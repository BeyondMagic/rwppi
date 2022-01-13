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

#include "main.hpp"

bool response_found = false;

/*
 * Local
 */

void
MethodLocal::math( const std::string & expression )
{

  const double answer = te_interp(expression.c_str(), 0);

  if (!std::isnan(answer))
  {
    RESPONSE(answer, "LocalMath");
  }

}

const bool
MethodLocal::all( const std::string & expression )
{

  // Local methods call-in.
  auto math_handler = std::async(&MethodLocal::math, this, expression);

  // Local methods call-out.
  math_handler.get();

  return response_found;

}

/*
 * Remote.
 */

void
MethodRemote::google_lyrics( const std::string & page )
{

  RESPONSE("found a nice lyrics response", "GoogleLyrics");

}

void
MethodRemote::google_math( const std::string & page )
{

  RESPONSE("found a nice math response", "GoogleMath");

}

/*
 * Compiled.
 */

const bool
MethodRemote::google( const std::string & page )
{

  // Local methods call-in.
  auto math_handler   = std::async(&MethodRemote::google_math,   this, page);
  auto lyrics_handler = std::async(&MethodRemote::google_lyrics, this, page);

  // Local methods call-out.
  math_handler.get();
  lyrics_handler.get();

  return response_found;

}
