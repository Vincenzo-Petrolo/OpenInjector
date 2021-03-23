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

#ifndef BINARY_H
#define BINARY_H

#include <sys/types.h>
#include <cstring>
#include <string>

typedef u_int8_t binary_t;

/**
 * Binary is a class to manage binary file.
 * It is responsible for the creation of the 
 * copies of the golden binary file.
 */
class Binary
{
public:
    /**
     * Default constructor
     * @param path : A string containing the path to the binary file.
     * @throw std::invalid_argument on wrong file path
     * @throw std::bad_alloc on error during allocation
     */
    Binary(std::string path);

    /**
     * Destructor
     */
    ~Binary();
    
    /**
     * Returns a copy to the golden binary file.
     * @return pointer to the new copy.
     * @warning IT MUST BE DESTROYD AFTER ITS USE.
     */
    binary_t *get_copy();
    
    /**
     * regenerate a copy of the goldenBinary.
     * @param pointer to the copy.
     */
    void regenerate(binary_t *copy);
    
    /**
     * Returns the size of the golden binary file.
     * @return the isze of the binary file.
     */ 
    size_t get_size();
    
    /**
     * Destroys the given copy pointer.
     * @param pointer to the copy to destroy.
     */ 
    void destroy_copy(binary_t *copyPtr);
    
    
private:
    
    /**
     * Pointer to the array that stores all the binary file.
    */
    binary_t *goldenBinary;
    
    /*
     * Size of the goldenBinary array in BYTES.
     */
    size_t size;

};

#endif // BINARY_H
