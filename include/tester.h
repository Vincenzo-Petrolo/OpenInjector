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

#ifndef TESTER_H
#define TESTER_H

#include "binary.h"
#include "statistics.h"
#include <list>

#define GOLDEN_TIME_TIMES 10
/**
 * Tester is a class responsible for the test
 * and statistics gathering over a binary file.
 */
class Tester
{
public:
	typedef enum {	O_DETAILED,
			O_SHORT
	} mode_t;
    /**
     * Default constructor.
     * Calculates all the parameters needed
     * for the comparison.
     * @param goldenBinary : pointer to the goldenBinary.
     */
    Tester(binary_t *goldenBinary,size_t size);

    /**
     * Destructor
     */
    ~Tester();
    
    /**
     * Wrapper for testing faulty binaries.
     * The statistics are held inside the class.
     * To receive them, use the getter.
     * @param target : The injected binary.
     */
    void testFaulty(binary_t *target,size_t size);
    
    /**
     * @return a Statistics object
     * containing all infos about
     * goldenBinary execution.
     */
    Statistics getGoldenStats();
    
    /**
     * @return a list of Statistics
     * containing all the informations
     * about executed binaries until the
     * call of this method, except the
     * golden statistics.
     */
    std::list<Statistics> getFaultyStats();
    
    /**
     * Write to file in csv format
     * all the statistics.
     * @param filename : desired filename.
     * @throw std::invalid_argument if a file
     * with that name already exists.
     */
    void writeStats(std::string filename);
    
    /**
     * Print to stdout statistics
     */
    void stringifyStats();
    
    /**Write statistics to @file "statistics.csv"
     * @param mode : O_DETAILED or O_SHORT
     * O_DETAILED : gives infos on all
     * the faulty binaries.
     * @warning O_DETAILED might generate
     * really huge files.
     * O_SHORT writes only statistics
     * related to average results.
     */
    void writeCSV(mode_t mode);
private:
    /**
     * The golden statistics.
     */
    Statistics *goldenStatistics;
    
    /**
     * Injected statistics.
     * TODO Linked lists are unpredictable becuase
     * they make use of pointers which are not branch
     * predictable so, try to use vector type instead
     * and see time comparison in simulation.
     */
    std::list<Statistics *> faultyStatistics;
    
    /**
     * Wrapper for the golden binary only.
     * @param goldenBinary : the goldenBinary.
     */
    void testGolden(binary_t *goldenBinary);
    
    /**
     * Must be called by wrappers.
     * @param target            : A pointer to the binary file.
     * @param statsContainer    : A pointer to a container of statistics_t, which will hold
     *                          all the infos after the test.
     */
    void test(binary_t *target,size_t size,Statistics *statsContainer);
    
    /**
     * @param mode : O_DETAILED || O_SHORT
     */
    std::string statsConstructor(mode_t mode);
};

#endif // TESTER_H
