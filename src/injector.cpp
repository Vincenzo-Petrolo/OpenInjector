/*
 * OpenInjector is an open source and free framework, used to run
 * fault injection campaigns on binary executables, and to test
 * their errors resilience.
 * Copyright (C) 2020  Vincenzo Petrolo - vincenzo@kernel-space.org
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "injector.h"
#include <iostream>

Injector::Injector()
{

}

Injector::~Injector()
{

}

void Injector::singleInjection(int i, u_int8_t* targetBinary)
{
    int array_pos = i/8;
    int byte_pos  = i%8;
    u_int8_t byte = targetBinary[array_pos];
    u_int8_t mask = 1 << (7-byte_pos);
    byte ^= mask;
    targetBinary[array_pos] = byte;
    
}
