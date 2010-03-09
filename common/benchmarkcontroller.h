/*
 * Copyright (C) 2009 Holger Hans Peter Freyther
 * Copyright (C) 2010 Benjamin Poulain, Nokia
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef BENCHMARKCONTROLLER_H
#define BENCHMARKCONTROLLER_H

#include "benchmark_p.h"
#include <QString>

class BenchmarkController {
public:
    BenchmarkController(const QString& testName, const QString& dataName, Benchmark* parent, int iterations = defaultIterations);
    ~BenchmarkController();

    void next();
    void timeNow();
    int iterations() const;

    int i;

    static void setDefaultIterations(int i) { defaultIterations = i; }
    static int defaultIterations;

private:
    long long timeElapsed() const;
    const int m_iterations;
    Benchmark m_benchmark;
    Benchmark* m_parent;
    bool m_timed;
};

// used to measure sub-sections of the code which is run multiple time
class SubSectionBenchmarkController {
public:
    SubSectionBenchmarkController(const QString& testName, const QString& dataName, Benchmark* parent, int iterations = 11);
    ~SubSectionBenchmarkController();
    void next();
    void startSubMeasure();
    void stopSubMeasure();
    int iterations() const { return m_iterations; }

    int i;
private:
    const int m_iterations;
    Benchmark m_benchmark;
    Benchmark* m_parent;
    bool m_running;
    unsigned int m_iterationTime;
};

#endif // BENCHMARKCONTROLLER_H
