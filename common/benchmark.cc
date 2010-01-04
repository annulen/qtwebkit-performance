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

BenchmarkController::BenchmarkController(const QString& name, Benchmark* parent, int iterations)
    : i(0)
    , m_iterations(iterations)
    , m_benchmark(name)
    , m_parent(parent)
{
    clock_gettime(CLOCK_MONOTONIC, &m_start);
}

BenchmarkController::~BenchmarkController()
{
    m_parent->addBenchmark(m_benchmark);
}


void BenchmarkController::next()
{
    m_benchmark.addResult(timeElapsed());
    ++i;
    clock_gettime(CLOCK_MONOTONIC, &m_start);
}

int BenchmarkController::iterations() const
{
    return m_iterations;
}

long long BenchmarkController::timeElapsed() const
{
    struct timespec _end;
    clock_gettime(CLOCK_MONOTONIC, &_end);

    long long start = m_start.tv_sec * 1000000000 + m_start.tv_nsec;
    long long end = _end.tv_sec * 1000000000 + _end.tv_nsec;

    return qAbs(end - start);
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

long long benchmarkOutput(const Benchmark& parent, const QString& indent)
{
    long long total_mean = 0;

    printf("%sPrinting result for: %s\n", qPrintable(indent), qPrintable(parent.name()));
    foreach(Benchmark bench, parent.benchmarks()) {
        if (!bench.benchmarks().isEmpty()) {
            total_mean += benchmarkOutput(bench, indent + "\t");
        }

        long long run = benchmarkMean(bench);
        printf("%s\tbenchmark: %s\n"
               "%s\t\tmean: %12lld nsecs\n"
               "%s\t\tavg:  %12lld nsecs\n",
                qPrintable(indent), qPrintable(bench.name()),
                qPrintable(indent), run,
                qPrintable(indent), benchmarkAverage(bench));
        total_mean += run;
    }

    if (indent.isEmpty())
        printf("Total mean: %12lld\n", total_mean);
    return total_mean;
}

Benchmark* benchmark_parent = new Benchmark("total");
