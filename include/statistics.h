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

#ifndef STATISTICS_H
#define STATISTICS_H

/**
 * Class that holds statistics
 * about the injected binary
 * execution.
 */
class Statistics
{
public:

    /**
     * Default constructor
     */
    Statistics();

    /**
     * Destructor
     */
    ~Statistics();

    /**
     * Implementing operation
     * if one wants to compare
     * the various statistics.
     *
     * @param other TODO
     * @return TODO
     */
    bool operator==(const Statistics& other) const;

    /**
     * Implementing operation
     * if one wants to compare
     * the various statistics.
     *
     * @param other TODO
     * @return TODO
     */
    bool operator!=(const Statistics& other) const;
    
    /**
     * @return time of execution.
     */ 
    double getTime();
    
    /**
     * @param time : time of execution to set.
     */
    void setTime(double time);
    
    int getExitStatus();
    
    void setExitStatus(int status);
private:
    double time;
    
    /*Contains a result object which describes
     * the exit code, stack trace, resource usage,
     * registers etc..
     */
    int exit_status;
};

#endif // STATISTICS_H
