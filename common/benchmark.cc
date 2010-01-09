/*
 * Copyright (C) 2010 Holger Hans Peter Freyther
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "benchmark.h"

#include <QDebug>
#include <math.h>
#include <signal.h>

#include <time.h>
#include <sys/time.h>

#define MSECS(t) (t/1000000)

static unsigned int sig_prof = 0;

static void sig_profiling(int)
{
    ++sig_prof;
}

static void startTimer()
{
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

BenchmarkController::BenchmarkController(const QString& name, Benchmark* parent, int iterations)
    : i(0)
    , m_iterations(iterations)
    , m_benchmark(name)
    , m_parent(parent)
    , m_timed(false)
{
    stopTimer();
    sig_prof = 0;
    startTimer();
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

    stopTimer();
    sig_prof = 0;
    startTimer();
}

int BenchmarkController::iterations() const
{
    return m_iterations;
}

long long BenchmarkController::timeElapsed() const
{
    stopTimer();
    return sig_prof;
}

void BenchmarkController::timeNow()
{
    if (i != 0)
        m_benchmark.addResult(timeElapsed());
    m_timed = true;
}

Benchmark::Benchmark()
    : m_empty(true)
{}

Benchmark::Benchmark(const QString& name)
    : m_empty(false)
    , m_name(name)
{
}

QString Benchmark::name() const
{
    return m_name;
}

bool Benchmark::isEmpty() const
{
    return m_empty;
}

void Benchmark::addBenchmark(const Benchmark& benchmark)
{
    if (!m_results.isEmpty())
        qFatal("You can not add benchmarks and results.");
    m_benchmarks.append(benchmark);
}

QList<Benchmark> Benchmark::benchmarks() const
{
    return m_benchmarks;
}

void Benchmark::addResult(long long result)
{
    if (!m_benchmarks.isEmpty())
        qFatal("You can not add benchmarks and results.");

    m_results.append(result);
}

QList<long long> Benchmark::results() const
{
    return m_results;
}

long long benchmarkMean(const Benchmark& _results)
{
    QList<long long> results = _results.results();
    qSort(results);

    return results.at(results.size() / 2);
}

long long benchmarkAverage(const Benchmark& _results)
{
    long long res = 0;
    foreach(long long val, _results.results())
        res += val;

    return res / _results.results().size();
}

SummaryResult benchmarkOutput(const Benchmark& parent, const QString& indent)
{
    SummaryResult result;

    printf("%sPrinting result for: %s\n", qPrintable(indent), qPrintable(parent.name()));
    foreach(Benchmark bench, parent.benchmarks()) {
        if (!bench.benchmarks().isEmpty()) {
            SummaryResult sum = benchmarkOutput(bench, indent + "\t");
            result.mean += sum.mean;
            result.average += sum.average;
        } else {
            long long run = benchmarkMean(bench);
            long long avg = benchmarkAverage(bench);
            int stddev = abs(benchmarkStdDeviationBiased(bench));
            printf("%s\tbenchmark: %s\n"
                   "%s\t\tmean: %8lld msecs +/- %d msecs, +/- %f %%\n"
                   "%s\t\tavg:  %8lld msecs\n",
                   qPrintable(indent), qPrintable(bench.name()),
                   qPrintable(indent), run, stddev, (stddev * 100.0) / run,
                   qPrintable(indent), avg);
            printf("%s\t\t\t", qPrintable(indent));
            foreach(long long res, bench.results()) {
                printf("%lld, ", res);
            }
            printf("\n");
            result.mean += run;
            result.average += avg;
        }

        ++result.size;
    }

    if (indent.isEmpty())
        printf("Total mean: %8lld msecs per test, avg: %8lld\n",
                result.mean / result.size, result.average / result.size);
    return result;
}

double benchmarkStdDeviationBiased(const Benchmark& benchmark)
{
    long long mean = benchmarkMean(benchmark);
    long long squares = 0;

    foreach (long long res, benchmark.results()) {
        long long delta = res - mean;
        squares += (delta * delta);
        if (squares < 0)
            qFatal("numbers wrapped");
    }

    double variance = benchmark.results().size() - 1.0;
    variance = squares / variance;
    return sqrt(variance);
}

Benchmark* benchmark_parent = new Benchmark("total");
