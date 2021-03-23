/*
 * OpenInjector is an open source and free framework, used to run
 * fault injection campaigns on binary executables, and to test
 * their errors resilience.
 * Copyright (C) 2020  Vincenzo Petrolo - vincenzo@kernel-space.org
 *  * 
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

#include "binary.h"
#include <iostream>
#include <exception>

Binary::Binary(std::string path)
{
    FILE *fp = fopen(path.c_str(),"rb");
    
    if (fp == NULL) {
        throw new std::invalid_argument("Invalid path to file name");
    }
    
    fseek(fp,0L,SEEK_END);
    
    this->size = ftell(fp);
    
    rewind(fp);

    this->goldenBinary = new binary_t[this->size];
    
    if (this->goldenBinary == nullptr) {
        throw std::bad_alloc();
    }
    fread(this->goldenBinary,sizeof(u_int8_t),this->size,fp);
    fclose(fp);
}

Binary::~Binary()
{
    delete this->goldenBinary;
}

binary_t *Binary::get_copy()
{
    u_int8_t *copy = new u_int8_t[this->size];
    if (copy == NULL) {
        throw new std::bad_alloc();
    }
    memcpy((void *) copy,(void *) this->goldenBinary,this->size);
    return copy;
}

void Binary::regenerate(binary_t* copy)
{
    memcpy(copy,this->goldenBinary,this->size);
}


size_t Binary::get_size()
{
    return this->size;
}

void Binary::destroy_copy(binary_t *copyPtr) 
{
    delete copyPtr;
}



