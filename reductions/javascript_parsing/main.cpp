/*
 * Copyright (C) 2010 Guillaume Chereau
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

#include <QtTest/QtTest>
#include <QDebug>
#include <QApplication>

#include "benchmark.h"

#include <JSGlobalData.h>
#include <InitializeThreading.h>
#include <Parser.h>
#include <UString.h>
#include <Lexer.h>

#ifndef yyparse
extern int jscyyparse(void*);
#endif

// Class used to parse the JavaScript code fragments.
class Parser
{
public:
    Parser()
    {
        JSC::initializeThreading();
        m_globalData = JSC::JSGlobalData::create().releaseRef();
        Q_ASSERT(m_globalData->parser);
    }

    int parse(const QString& fragment)
    {
        // Set up the parsing context.  Hopefully this is fast
        // compared to the actual parsing operation.
        JSC::UString string((const UChar*)fragment.constData(), fragment.size());
        WTF::PassRefPtr<JSC::UStringSourceProvider> provider = JSC::UStringSourceProvider::create(string, "");
        JSC::SourceCode code(provider, 0);
        JSC::ParserArena arena;
        m_globalData->lexer->setCode(code, arena);

        int parseError = jscyyparse(m_globalData);
        m_globalData->lexer->clear();
        return parseError;
    }

private:
    JSC::JSGlobalData* m_globalData;
};

class tst_JavaScriptParsing : public QObject {
    Q_OBJECT

public:
    ~tst_JavaScriptParsing()
    {
         benchmarkOutput(*benchmark_parent);
    }

private Q_SLOTS:
    void initTestCase();
    void testParsing();

private:
    // Contains all the JavaScript code fragments.
    QStringList fragments;
    Parser parser;
};

void tst_JavaScriptParsing::initTestCase()
{
    // Load all the fragments from the data file.  In the file the
    // fragments are delimited by the line : "NEW FRAGMENT".
    QFile file("data.txt");
    if (!file.open(QIODevice::ReadOnly))
        QFAIL("Can not open the data file \"data.txt\".");
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString all = in.readAll();
    fragments = all.split("\nNEW FRAGMENT\n");
}

void tst_JavaScriptParsing::testParsing()
{
    WEB_BENCHMARK("javascript-parsing") {
        foreach(QString fragment, fragments) {
            parser.parse(fragment);
        }
    }
}

QTEST_MAIN(tst_JavaScriptParsing)
#include "main.moc"
