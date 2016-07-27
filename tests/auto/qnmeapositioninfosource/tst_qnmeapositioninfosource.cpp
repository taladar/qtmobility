/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

//TESTED_COMPONENT=src/location

#include "tst_qnmeapositioninfosource.h"

tst_QNmeaPositionInfoSource::tst_QNmeaPositionInfoSource(QNmeaPositionInfoSource::UpdateMode mode, QObject *parent)
    : QObject(parent),
      m_mode(mode)
{
}

void tst_QNmeaPositionInfoSource::initTestCase()
{
    qRegisterMetaType<QGeoPositionInfo>();
    qRegisterMetaType<QNmeaPositionInfoSource::UpdateMode>();
}

void tst_QNmeaPositionInfoSource::constructor()
{
    QObject o;
    QNmeaPositionInfoSource source(m_mode, &o);
    QCOMPARE(source.updateMode(), m_mode);
    QCOMPARE(source.parent(), &o);
}

void tst_QNmeaPositionInfoSource::supportedPositioningMethods()
{
    QNmeaPositionInfoSource source(m_mode);
    QCOMPARE(source.supportedPositioningMethods(), QNmeaPositionInfoSource::SatellitePositioningMethods);
}

void tst_QNmeaPositionInfoSource::minimumUpdateInterval()
{
    QNmeaPositionInfoSource source(m_mode);
    QCOMPARE(source.minimumUpdateInterval(), 100);
}

void tst_QNmeaPositionInfoSource::setUpdateInterval_delayedUpdate()
{
    // If an update interval is set, and an update is not available at a
    // particular interval, the source should emit the next update as soon
    // as it becomes available

    QNmeaPositionInfoSource source(m_mode);
    QNmeaPositionInfoSourceProxyFactory factory;
    QNmeaPositionInfoSourceProxy *proxy = static_cast<QNmeaPositionInfoSourceProxy*>(factory.createProxy(&source));

    QSignalSpy spyUpdate(proxy->source(), SIGNAL(positionUpdated(QGeoPositionInfo)));
    proxy->source()->setUpdateInterval(500);
    proxy->source()->startUpdates();

    QTest::qWait(600);
    QDateTime now = QDateTime::currentDateTime();
    proxy->feedUpdate(now);
    QTRY_COMPARE(spyUpdate.count(), 1);

    // should have gotten the update immediately, and not have needed to
    // wait until the next interval
    QVERIFY(now.time().msecsTo(QDateTime::currentDateTime().time()) < 200);
}

void tst_QNmeaPositionInfoSource::lastKnownPosition()
{
    QNmeaPositionInfoSource source(m_mode);
    QNmeaPositionInfoSourceProxyFactory factory;
    QNmeaPositionInfoSourceProxy *proxy = static_cast<QNmeaPositionInfoSourceProxy*>(factory.createProxy(&source));

    QCOMPARE(proxy->source()->lastKnownPosition(), QGeoPositionInfo());

    // source may need requestUpdate() or startUpdates() to be called to
    // trigger reading of data channel
    QSignalSpy spyTimeout(proxy->source(), SIGNAL(updateTimeout()));
    proxy->source()->requestUpdate(proxy->source()->minimumUpdateInterval());
    QTRY_COMPARE(spyTimeout.count(), 1);

    // If an update is received and startUpdates() or requestUpdate() hasn't
    // been called, it should still be available through lastKnownPosition()
    QDateTime dt = QDateTime::currentDateTime().toUTC();
    proxy->feedUpdate(dt);
    QTRY_COMPARE(proxy->source()->lastKnownPosition().timestamp(), dt);

    QList<QDateTime> dateTimes = createDateTimes(5);
    for (int i=0; i<dateTimes.count(); i++) {
        proxy->source()->requestUpdate();
        proxy->feedUpdate(dateTimes[i]);
        QTRY_COMPARE(proxy->source()->lastKnownPosition().timestamp(), dateTimes[i]);
    }

    proxy->source()->startUpdates();
    dateTimes = createDateTimes(5);
    for (int i=0; i<dateTimes.count(); i++) {
        proxy->feedUpdate(dateTimes[i]);
        QTRY_COMPARE(proxy->source()->lastKnownPosition().timestamp(), dateTimes[i]);
    }
}

void tst_QNmeaPositionInfoSource::beginWithBufferedData()
{
    // In SimulationMode, data stored in the QIODevice is read when
    // startUpdates() or requestUpdate() is called.
    // In RealTimeMode, all existing data in the QIODevice is ignored -
    // only new data will be read.

    QFETCH(QList<QDateTime>, dateTimes);
    QFETCH(UpdateTriggerMethod, trigger);

    QByteArray bytes;
    for (int i=0; i<dateTimes.count(); i++)
        bytes += QLocationTestUtils::createRmcSentence(dateTimes[i]).toLatin1();
    QBuffer buffer;
    buffer.setData(bytes);

    QNmeaPositionInfoSource source(m_mode);
    QSignalSpy spy(&source, SIGNAL(positionUpdated(QGeoPositionInfo)));
    source.setDevice(&buffer);

    if (trigger == StartUpdatesMethod)
        source.startUpdates();
    else if (trigger == RequestUpdatesMethod)
        source.requestUpdate();

    if (m_mode == QNmeaPositionInfoSource::RealTimeMode) {
        QTest::qWait(300);
        QCOMPARE(spy.count(), 0);
    } else {
        if (trigger == StartUpdatesMethod) {
            QTRY_COMPARE(spy.count(), dateTimes.count());
            for (int i=0; i<dateTimes.count(); i++)
                QCOMPARE(spy.at(i).at(0).value<QGeoPositionInfo>().timestamp(), dateTimes[i]);
        } else if (trigger == RequestUpdatesMethod) {
            QTRY_COMPARE(spy.count(), 1);
            QCOMPARE(spy.at(0).at(0).value<QGeoPositionInfo>().timestamp(), dateTimes.first());
        }
    }
}

void tst_QNmeaPositionInfoSource::beginWithBufferedData_data()
{
    QTest::addColumn<QList<QDateTime> >("dateTimes");
    QTest::addColumn<UpdateTriggerMethod>("trigger");

    QList<QDateTime> dateTimes;
    dateTimes << QDateTime::currentDateTime().toUTC();

    QTest::newRow("startUpdates(), 1 update in buffer") << dateTimes << StartUpdatesMethod;
    QTest::newRow("requestUpdate(), 1 update in buffer") << dateTimes << RequestUpdatesMethod;

    for (int i=1; i<3; i++)
        dateTimes << dateTimes[0].addDays(i);
    QTest::newRow("startUpdates(), multiple updates in buffer") << dateTimes << StartUpdatesMethod;
    QTest::newRow("requestUpdate(), multiple updates in buffer") << dateTimes << RequestUpdatesMethod;
}

void tst_QNmeaPositionInfoSource::startUpdates()
{
    QFETCH(QList<QDateTime>, dateTimes);

    QNmeaPositionInfoSource source(m_mode);
    QNmeaPositionInfoSourceProxyFactory factory;
    QNmeaPositionInfoSourceProxy *proxy = static_cast<QNmeaPositionInfoSourceProxy*>(factory.createProxy(&source));

    QSignalSpy spyUpdate(proxy->source(), SIGNAL(positionUpdated(QGeoPositionInfo)));
    proxy->source()->startUpdates();

    for (int i=0; i<dateTimes.count(); i++)
        proxy->feedUpdate(dateTimes[i]);
    QTRY_COMPARE(spyUpdate.count(), dateTimes.count());
}

void tst_QNmeaPositionInfoSource::startUpdates_data()
{
    QTest::addColumn<QList<QDateTime> >("dateTimes");

    QTest::newRow("1 update") << createDateTimes(1);
    QTest::newRow("2 updates") << createDateTimes(2);
    QTest::newRow("10 updates") << createDateTimes(10);
}

void tst_QNmeaPositionInfoSource::startUpdates_withTimeout()
{
    QNmeaPositionInfoSource source(m_mode);
    QNmeaPositionInfoSourceProxyFactory factory;
    QNmeaPositionInfoSourceProxy *proxy = static_cast<QNmeaPositionInfoSourceProxy*>(factory.createProxy(&source));

    QSignalSpy spyUpdate(proxy->source(), SIGNAL(positionUpdated(QGeoPositionInfo)));
    QSignalSpy spyTimeout(proxy->source(), SIGNAL(updateTimeout()));

    proxy->source()->setUpdateInterval(1000);
    proxy->source()->startUpdates();

    QDateTime dt = QDateTime::currentDateTime().toUTC();    

    if (m_mode == QNmeaPositionInfoSource::SimulationMode) {
        // the first sentence primes the simulation
        proxy->feedBytes(QLocationTestUtils::createRmcSentence(dt).toLatin1());
        proxy->feedBytes(QLocationTestUtils::createRmcSentence(dt.addMSecs(50)).toLatin1());
        proxy->feedBytes(QLocationTestUtils::createRmcSentence(dt.addSecs(1)).toLatin1());
        proxy->feedBytes(QLocationTestUtils::createRmcSentence(dt.addSecs(2)).toLatin1());
        proxy->feedBytes(QLocationTestUtils::createRmcSentence(dt.addSecs(9)).toLatin1());

        int i = 0;

        for (int j = 0; j < 3; ++j) {
            i = 0;
            for (; i < 12; ++i) {
                QTest::qWait(100);
                if ((spyUpdate.count() == 1) && (spyTimeout.count() == 0))
                    break;
            }
            QVERIFY((spyUpdate.count() == 1) && (spyTimeout.count() == 0));
            spyUpdate.clear();
            for (; i < 10; ++i) {
                QTest::qWait(100);
            }
        }
       
        i = 0;
        for (; i < 72; ++i) {
            QTest::qWait(100);
            if ((spyUpdate.count() == 0) && (spyTimeout.count() == 1))
                break;
        }
        QVERIFY((spyUpdate.count() == 0) && (spyTimeout.count() == 1));
        spyTimeout.clear();

        for (; i < 72; ++i) {
            QTest::qWait(100);
            if ((spyUpdate.count() == 1) && (spyTimeout.count() == 0))
                break;
        }
        QVERIFY((spyUpdate.count() == 1) && (spyTimeout.count() == 0));

    } else {
        QTest::qWait(900);
        // dt + 900    
        QVERIFY(spyUpdate.count() == 0 && spyTimeout.count() == 0);
        proxy->feedBytes(QLocationTestUtils::createRmcSentence(dt.addSecs(1)).toLatin1());    
        QTest::qWait(200);
        // dt + 1100
        QVERIFY(spyUpdate.count() == 1 && spyTimeout.count() == 0);
        spyUpdate.clear();

        QTest::qWait(800);
        // dt + 1900    
        QVERIFY(spyUpdate.count() == 0 && spyTimeout.count() == 0);
        proxy->feedBytes(QLocationTestUtils::createRmcSentence(dt.addSecs(2)).toLatin1());    
        QTest::qWait(200);
        // dt + 2100
        QVERIFY(spyUpdate.count() == 1 && spyTimeout.count() == 0);
        spyUpdate.clear();

        QTest::qWait(800);
        // dt + 2900    
        QVERIFY(spyUpdate.count() == 0 && spyTimeout.count() == 0);
        proxy->feedBytes(QLocationTestUtils::createRmcSentence(dt.addSecs(3)).toLatin1());    
        QTest::qWait(200);
        // dt + 3100
        QVERIFY(spyUpdate.count() == 1 && spyTimeout.count() == 0);
        spyUpdate.clear();

        QTest::qWait(3800);
        // dt + 6900    
        QVERIFY(spyUpdate.count() == 0 && spyTimeout.count() == 1);
        spyTimeout.clear();

        proxy->feedBytes(QLocationTestUtils::createRmcSentence(dt.addSecs(7)).toLatin1());    
        QTest::qWait(200);
        // dt + 7100
        QVERIFY(spyUpdate.count() == 1 && spyTimeout.count() == 0);
        spyUpdate.clear();
    }
}

void tst_QNmeaPositionInfoSource::startUpdates_expectLatestUpdateOnly()
{
    // If startUpdates() is called and an interval has been set, if multiple'
    // updates are in the buffer, only the latest update should be emitted

    QNmeaPositionInfoSource source(m_mode);
    QNmeaPositionInfoSourceProxyFactory factory;
    QNmeaPositionInfoSourceProxy *proxy = static_cast<QNmeaPositionInfoSourceProxy*>(factory.createProxy(&source));

    QSignalSpy spyUpdate(proxy->source(), SIGNAL(positionUpdated(QGeoPositionInfo)));
    proxy->source()->setUpdateInterval(500);
    proxy->source()->startUpdates();

    QList<QDateTime> dateTimes = createDateTimes(3);
    for (int i=0; i<dateTimes.count(); i++)
        proxy->feedUpdate(dateTimes[i]);

    QTRY_COMPARE(spyUpdate.count(), 1);
    QCOMPARE(spyUpdate[0][0].value<QGeoPositionInfo>().timestamp(), dateTimes.last());
}

void tst_QNmeaPositionInfoSource::startUpdates_waitForValidDateTime()
{
    // Tests that the class does not emit an update until it receives a
    // sentences with a valid date *and* time. All sentences before this
    // should be ignored, and any sentences received after this that do
    // not have a date should use the known date.

    QFETCH(QByteArray, bytes);
    QFETCH(QList<QDateTime>, dateTimes);

    QNmeaPositionInfoSource source(m_mode);
    QNmeaPositionInfoSourceProxyFactory factory;
    QNmeaPositionInfoSourceProxy *proxy = static_cast<QNmeaPositionInfoSourceProxy*>(factory.createProxy(&source));

    QSignalSpy spy(proxy->source(), SIGNAL(positionUpdated(QGeoPositionInfo)));
    proxy->source()->startUpdates();

    proxy->feedBytes(bytes);
    QTRY_COMPARE(spy.count(), dateTimes.count());

    for (int i=0; i<spy.count(); i++)
        QCOMPARE(spy[i][0].value<QGeoPositionInfo>().timestamp(), dateTimes[i]);
}

void tst_QNmeaPositionInfoSource::startUpdates_waitForValidDateTime_data()
{
    QTest::addColumn<QByteArray>("bytes");
    QTest::addColumn<QList<QDateTime> >("dateTimes");

    QDateTime dt = QDateTime::currentDateTime().toUTC();
    QByteArray bytes;

    // should only receive RMC sentence and the GGA sentence *after* it
    bytes += QLocationTestUtils::createGgaSentence(dt.addSecs(1).time()).toLatin1();
    bytes += QLocationTestUtils::createRmcSentence(dt.addSecs(2)).toLatin1();
    bytes += QLocationTestUtils::createGgaSentence(dt.addSecs(3).time()).toLatin1();
    QTest::newRow("Feed GGA,RMC,GGA; expect RMC, second GGA only")
            << bytes << (QList<QDateTime>() << dt.addSecs(2) << dt.addSecs(3));

    // should not receive ZDA (has no coordinates) but should get the GGA
    // sentence after it since it got the date/time from ZDA
    bytes.clear();
    bytes += QLocationTestUtils::createGgaSentence(dt.addSecs(1).time()).toLatin1();
    bytes += QLocationTestUtils::createZdaSentence(dt.addSecs(2)).toLatin1();
    bytes += QLocationTestUtils::createGgaSentence(dt.addSecs(3).time()).toLatin1();
    QTest::newRow("Feed GGA,ZDA,GGA; expect second GGA only")
            << bytes << (QList<QDateTime>() << dt.addSecs(3));

    if (m_mode == QNmeaPositionInfoSource::SimulationMode) {
        // In sim m_mode, should ignore sentence with a date/time before the known date/time
        // (in real time m_mode, everything is passed on regardless)
        bytes.clear();
        bytes += QLocationTestUtils::createRmcSentence(dt.addSecs(1)).toLatin1();
        bytes += QLocationTestUtils::createRmcSentence(dt.addSecs(-2)).toLatin1();
        bytes += QLocationTestUtils::createRmcSentence(dt.addSecs(2)).toLatin1();
        QTest::newRow("Feed good RMC, RMC with bad date/time, good RMC; expect first and third RMC only")
                << bytes << (QList<QDateTime>() << dt.addSecs(1) << dt.addSecs(2));
    }
}

void tst_QNmeaPositionInfoSource::requestUpdate_waitForValidDateTime()
{
    QFETCH(QByteArray, bytes);
    QFETCH(QList<QDateTime>, dateTimes);

    QNmeaPositionInfoSource source(m_mode);
    QNmeaPositionInfoSourceProxyFactory factory;
    QNmeaPositionInfoSourceProxy *proxy = static_cast<QNmeaPositionInfoSourceProxy*>(factory.createProxy(&source));

    QSignalSpy spy(proxy->source(), SIGNAL(positionUpdated(QGeoPositionInfo)));
    proxy->source()->requestUpdate();

    proxy->feedBytes(bytes);
    QTRY_COMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0].value<QGeoPositionInfo>().timestamp(), dateTimes[0]);
}

void tst_QNmeaPositionInfoSource::requestUpdate_waitForValidDateTime_data()
{
    startUpdates_waitForValidDateTime_data();
}

void tst_QNmeaPositionInfoSource::requestUpdate()
{
    QNmeaPositionInfoSource source(m_mode);
    QNmeaPositionInfoSourceProxyFactory factory;
    QNmeaPositionInfoSourceProxy *proxy = static_cast<QNmeaPositionInfoSourceProxy*>(factory.createProxy(&source));

    QSignalSpy spyUpdate(proxy->source(), SIGNAL(positionUpdated(QGeoPositionInfo)));
    QSignalSpy spyTimeout(proxy->source(), SIGNAL(updateTimeout()));
    QDateTime dt;

    proxy->source()->requestUpdate(100);
    QTRY_COMPARE(spyTimeout.count(), 1);
    spyTimeout.clear();

    dt = QDateTime::currentDateTime().toUTC();
    proxy->feedUpdate(dt);
    proxy->source()->requestUpdate();
    QTRY_COMPARE(spyUpdate.count(), 1);
    QCOMPARE(spyUpdate[0][0].value<QGeoPositionInfo>().timestamp(), dt);
    QCOMPARE(spyTimeout.count(), 0);
    spyUpdate.clear();

    // delay the update and expect it to be emitted after 300ms
    dt = QDateTime::currentDateTime().toUTC();
    proxy->source()->requestUpdate(1000);
    QTest::qWait(300);
    proxy->feedUpdate(dt);
    QTRY_COMPARE(spyUpdate.count(), 1);
    QCOMPARE(spyUpdate[0][0].value<QGeoPositionInfo>().timestamp(), dt);
    QCOMPARE(spyTimeout.count(), 0);
    spyUpdate.clear();

    // delay the update and expect updateTimeout() to be emitted
    dt = QDateTime::currentDateTime().toUTC();
    proxy->source()->requestUpdate(500);
    QTest::qWait(1000);
    proxy->feedUpdate(dt);
    QCOMPARE(spyTimeout.count(), 1);
    QCOMPARE(spyUpdate.count(), 0);
    spyUpdate.clear();
}

void tst_QNmeaPositionInfoSource::requestUpdate_after_start()
{
    QNmeaPositionInfoSource source(m_mode);
    QNmeaPositionInfoSourceProxyFactory factory;
    QNmeaPositionInfoSourceProxy *proxy = static_cast<QNmeaPositionInfoSourceProxy*>(factory.createProxy(&source));

    QSignalSpy spyUpdate(proxy->source(), SIGNAL(positionUpdated(QGeoPositionInfo)));
    QSignalSpy spyTimeout(proxy->source(), SIGNAL(updateTimeout()));

    // Start updates with 500ms interval and requestUpdate() with 100ms
    // timeout. Feed an update, and it should be emitted immediately due to
    // the requestUpdate(). The update should not be emitted again after that
    // (i.e. the startUpdates() interval should not cause it to be re-emitted).

    QDateTime dt = QDateTime::currentDateTime().toUTC();
    proxy->source()->setUpdateInterval(500);
    proxy->source()->startUpdates();
    proxy->source()->requestUpdate(100);
    proxy->feedUpdate(dt);
    QTRY_COMPARE(spyUpdate.count(), 1);
    QCOMPARE(spyUpdate[0][0].value<QGeoPositionInfo>().timestamp(), dt);
    QCOMPARE(spyTimeout.count(), 0);
    spyUpdate.clear();

    // Update has been emitted for requestUpdate(), shouldn't be emitted for startUpdates()
    QTest::qWait(1000);
    QCOMPARE(spyUpdate.count(), 0);
}

void tst_QNmeaPositionInfoSource::testWithBadNmea()
{
    QFETCH(QByteArray, bytes);
    QFETCH(QList<QDateTime>, dateTimes);
    QFETCH(UpdateTriggerMethod, trigger);

    QNmeaPositionInfoSource source(m_mode);
    QNmeaPositionInfoSourceProxyFactory factory;
    QNmeaPositionInfoSourceProxy *proxy = static_cast<QNmeaPositionInfoSourceProxy*>(factory.createProxy(&source));

    QSignalSpy spy(proxy->source(), SIGNAL(positionUpdated(QGeoPositionInfo)));
    if (trigger == StartUpdatesMethod)
        proxy->source()->startUpdates();
    else
        proxy->source()->requestUpdate();

    proxy->feedBytes(bytes);
    QTRY_COMPARE(spy.count(), dateTimes.count());
    for (int i=0; i<dateTimes.count(); i++)
        QCOMPARE(spy[i][0].value<QGeoPositionInfo>().timestamp(), dateTimes[i]);
}

void tst_QNmeaPositionInfoSource::testWithBadNmea_data()
{
    QTest::addColumn<QByteArray>("bytes");
    QTest::addColumn<QList<QDateTime> >("dateTimes");
    QTest::addColumn<UpdateTriggerMethod>("trigger");

    QDateTime firstDateTime = QDateTime::currentDateTime().toUTC();
    QByteArray bad = QLocationTestUtils::createRmcSentence(firstDateTime.addSecs(1)).toLatin1();
    bad = bad.mid(bad.length()/2);
    QDateTime lastDateTime = firstDateTime.addSecs(2);

    QByteArray bytes;
    bytes += QLocationTestUtils::createRmcSentence(firstDateTime).toLatin1();
    bytes += bad;
    bytes += QLocationTestUtils::createRmcSentence(lastDateTime).toLatin1();
    QTest::newRow("requestUpdate(), bad second sentence") << bytes
            << (QList<QDateTime>() << firstDateTime) << RequestUpdatesMethod;
    QTest::newRow("startUpdates(), bad second sentence") << bytes
            << (QList<QDateTime>() << firstDateTime << lastDateTime) << StartUpdatesMethod;
}