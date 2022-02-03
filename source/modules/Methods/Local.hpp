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

MethodLocal::MethodLocal( const std::string & data )
{

  this->expression = data;

}

void
MethodLocal::Math()
{

  context_t ctx;

  // 1. Interpret the expression with the minimal library "tinyexpr".
  const double answer = te_interp(this->expression.c_str(), 0);

  // 2. If it's not a number (there's no valid answer for the expression).
  if ( !std::isnan(answer) ) {

    std::cout << answer << "\n";

    response_found = true;

  }

}

void
MethodLocal::All()
{

  // 1. Local methods call-in to start the thread.
  auto math_handler = std::async(&MethodLocal::Math, this);

  // 2. Local methods call-out to finish the thread.
  math_handler.get();

}
