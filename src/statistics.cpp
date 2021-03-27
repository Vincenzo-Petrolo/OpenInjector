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

#include "statistics.h"

Statistics::Statistics()
{
	this->time = 0;
}

Statistics::~Statistics()
{

}

bool Statistics::operator==(const Statistics& other) const
{
    return false;
}

bool Statistics::operator!=(const Statistics& other) const
{
    return false;
}

double Statistics::getTime()
{
    return this->time;
}

void Statistics::setTime(double time)
{
    this->time = time;
}

int Statistics::getExitStatus()
{
	return this->exit_status;
}

void Statistics::setExitStatus(int status)
{
	this->exit_status = status;
}
