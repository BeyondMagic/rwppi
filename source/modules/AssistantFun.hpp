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

#include <string>

/*
 * This is a simple trivial non-performant affecter made for fun as Iris developmend can become quite tedious, you can always stop and just think of fun things you would like to see in certain exceptions or operands Iris cannot operate.
 */
class AITricks {

  public:

    // public:
    // Random item from an array.
    // const unsigned int random ( & char */**/ )

    AITricks ( void );

    // In case Iris tried to perform a mathematical expression locally and remotely and did not found any results for it.
    const std::string
    math ( void );

    // In case Iris received nothing.
    const std::string
    nothing ( void );

};

/*
 * Definitions...
 */

AITricks::AITricks ( void ) {}

const std::string
AITricks::math ( void )
{
  return "No, that's not how you put a mathematical expression onto Internet!";
}

const std::string
AITricks::nothing ( void )
{
  return "Haha, your query is so small! I can't even see it!";
  // What do you want me to do? Create information from out of thin electrons for you?"
}
