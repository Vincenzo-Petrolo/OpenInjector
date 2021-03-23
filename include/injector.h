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

#ifndef INJECTOR_H
#define INJECTOR_H

#include <sys/types.h>
/**
 * Injector class is responsible for injection.
 * It is done thanks to the pointer to the binary array
 * and each injection method must return the pointer
 * to an injected copy of the binary file.
 */
class Injector
{
public:
    /**
     * Default constructor
     */
    Injector();

    /**
     * Destructor
     */
    ~Injector();
    
    /**
     * Injects the target binary by flipping the i-th bit.
     * @param i             : Position of the bit to inject
     * @param targetBinary  : Pointer to the targetBinary
     */
    static void singleInjection(int i,u_int8_t *targetBinary);
};

#endif // INJECTOR_H
