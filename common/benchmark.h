/*
 * Copyright (C) 2010 Holger Hans Peter Freyther
 * Copyright (C) 2010 Balazs Kelemen, University of Szeged
 * Copyright (C) 2010 Benjamin Poulain, Nokia
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

#ifndef benchmark_h
#define benchmark_h

#include "benchmarkoutputwriter.h"
#include <QPair>
#include <QSharedPointer>
#include <QString>
#include <QTimer>

/**
 * Benchmark related functionality
 */

class Benchmark {
public:
    Benchmark(const QString& testName, const QString& dataName = QString());

    QString testName() const { return m_testName; }
    QString dataName() const { return m_dataName; }
    bool isEmpty() const;

    void addBenchmark(const Benchmark&);
    QList<Benchmark> benchmarks() const;

    void addResult(long long result);
    QList<long long> results() const;

private:
    bool m_empty;
    QString m_testName;
    QString m_dataName;
    QList<long long> m_results;
    QList<Benchmark> m_benchmarks;
};

class BenchmarkController {
public:
    BenchmarkController(const QString& testName, const QString& dataName, Benchmark *parent, int iterations = defaultIterations);
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
    SubSectionBenchmarkController(const QString& testName, const QString& dataName, Benchmark *parent, int iterations = 11);
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

extern QSharedPointer<BenchmarkOutputWriter> outWriter;

void benchmarkOutput();

/**
 * Starts an event loop that runs until the given signal is received.
 Optionally the event loop
 * can return earlier on a timeout.
 *
 * \return \p true if the requested signal was received
 *         \p false on timeout
 */
bool waitForSignal(QObject* obj, const char* signal, int timeout = 0);

/*
 * Use shadowing
 */
extern Benchmark* benchmark_parent;

/*
 * macros for benchmarking
 */
#define WEB_CREATE_GROUP(testName, dataName) \
    static Benchmark* old_parent = ::benchmark_parent; \
    static Benchmark* benchmark_parent = new Benchmark(testName, dataName); \
    ::old_parent->add(benchmark_parent);

#define WEB_BENCHMARK(testName, dataName) \
    for (BenchmarkController web__controller(testName, dataName, benchmark_parent); \
         web__controller.i < web__controller.iterations(); web__controller.next())

#define WEB_BENCHMARK_ITER(testName, dataName, iter) \
    for (BenchmarkController web__controller(testName, dataName, benchmark_parent, iter); \
         web__controller.i < web__controller.iterations(); web__controller.next())

#define TIME_NOW \
        web__controller.timeNow();

#define WEB_BENCHMARK_SUBSECTION(testName, dataName) \
    for (SubSectionBenchmarkController web__controller(testName, dataName, benchmark_parent); \
         web__controller.i < web__controller.iterations(); web__controller.next())

#endif
