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

#include "benchmarkcontroller.h"

#ifdef Q_OS_SYMBIAN

#include <e32base.h>
#include <e32std.h>
#include <HAL.h>

#define MSECS(t) (t)

static TInt getCounterFreq()
{
    static TInt freq = 0;
    if(!freq)
        HAL::Get(HALData::EFastCounterFrequency, freq);

    return freq;
}

TUint32 startTime = 0;

static unsigned int elapsedTime()
{
    TUint32 curTime = User::FastCounter();

    return abs(((qreal)(curTime - startTime) * 1000) / getCounterFreq());
}

static void startTimer()
{
    startTime = User::FastCounter();
}

static void stopTimer()
{
  // NOP for symbian
}

#else

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define MSECS(t) (t/1000000)

static unsigned int sig_prof = 0;

static void sig_profiling(int)
{
    ++sig_prof;
}

static unsigned int elapsedTime()
{
    return sig_prof;
}

static void startTimer()
{
    sig_prof = 0;
    struct itimerval tim;
    tim.it_interval.tv_sec = 0;
    tim.it_interval.tv_usec = 1;
    tim.it_value.tv_sec = 0;
    tim.it_value.tv_usec = 1;
    setitimer(ITIMER_PROF, &tim, 0);
}

static void stopTimer()
{
    setitimer(ITIMER_PROF, 0, 0);
}

static void __attribute__((constructor)) benchmark_init()
{
    struct sigaction sa;
    sa.sa_handler = sig_profiling;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGPROF, &sa, 0) != 0) {
        fprintf(stderr, "Failed to register signal handler.\n");
        exit(-1);
    }

}
#endif // Q_OS_SYMBIAN

static void resetTimer()
{
    stopTimer();
    startTimer();
}

int BenchmarkController::defaultIterations = 11;

BenchmarkController::BenchmarkController(const QString& testName, const QString& dataName, Benchmark *parent, int iterations)
    : i(0)
    , m_iterations(iterations)
    , m_benchmark(testName, dataName)
    , m_parent(parent)
    , m_timed(false)
{
    resetTimer();
}

BenchmarkController::~BenchmarkController()
{
    m_parent->addBenchmark(m_benchmark);
}


void BenchmarkController::next()
{
    if (!m_timed && i != 0)
        m_benchmark.addResult(timeElapsed());
    m_timed = false;
    ++i;

    resetTimer();
}

int BenchmarkController::iterations() const
{
    return m_iterations;
}

long long BenchmarkController::timeElapsed() const
{
    stopTimer();
    return elapsedTime();
}

void BenchmarkController::timeNow()
{
    if (i != 0)
        m_benchmark.addResult(timeElapsed());
    m_timed = true;
}

SubSectionBenchmarkController::SubSectionBenchmarkController(const QString& testName, const QString& dataName, Benchmark *parent, int iterations)
    : i(0)
    , m_iterations(iterations)
    , m_benchmark(testName, dataName)
    , m_parent(parent)
    , m_running(false)
    , m_iterationTime(0)
{
}

SubSectionBenchmarkController::~SubSectionBenchmarkController()
{
    m_parent->addBenchmark(m_benchmark);
}

void SubSectionBenchmarkController::next()
{
    stopSubMeasure();
    if (i != 0)
        m_benchmark.addResult(m_iterationTime);
    ++i;
    m_iterationTime = 0;
}

void SubSectionBenchmarkController::startSubMeasure()
{
    m_running = true;
    startTimer();
}

void SubSectionBenchmarkController::stopSubMeasure()
{
    if(m_running)
    {
        m_running = false;
        stopTimer();
        m_iterationTime =+ elapsedTime();
    }
}
