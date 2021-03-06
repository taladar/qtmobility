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

#include "qversitorganizerhandler.h"

QTM_USE_NAMESPACE

Q_DEFINE_LATIN1_CONSTANT(QVersitOrganizerHandlerFactory::ProfileSync, "Sync");
Q_DEFINE_LATIN1_CONSTANT(QVersitOrganizerHandlerFactory::ProfileBackup, "Backup");

/*!
  \class QVersitOrganizerHandler
  \brief The QVersitOrganizerHandler class is a union of the
  QVersitOrganizerImporterPropertyHandlerV2 and QVersitOrganizerExporterDetailHandlerV2 interfaces.
  \ingroup versit-extension
  \inmodule QtVersit
  \since 1.1
 */

/*!
  \class QVersitOrganizerHandlerFactory
  \brief The QVersitOrganizerHandlerFactory class provides the interface for Versit plugins.
  \ingroup versit-extension
  \inmodule QtVersit
    \since 1.2

  This class provides a simple interface for the creation of QVersitOrganizerHandler instances.
  Implement this interface to write a Versit plugin.  For more details, see \l{Versit Plugins}.
 */

/*!
   \variable QVersitOrganizerHandlerFactory::ProfileSync

   The constant string signifying a plugin that is relevant to import and export in a
   synchronization context.
   \sa QVersitOrganizerHandlerFactory::profiles(), QVersitOrganizerImporter, QVersitOrganizerExporter
 */

/*!
   \variable QVersitOrganizerHandlerFactory::ProfileBackup

   The constant string signifying a plugin that is relevant to import and export in a backup/restore
   context.
   \sa profiles(),
   QVersitOrganizerImporter::QVersitOrganizerImporter(),
   QVersitOrganizerExporter::QVersitOrganizerExporter()
 */

/*!
  \fn QVersitOrganizerHandlerFactory::~QVersitOrganizerHandlerFactory()
  This frees any memory used by the QVersitOrganizerHandlerFactory.
 */

/*!
  \fn QSet<QString> QVersitOrganizerHandlerFactory::profiles() const
  This function can be overridden to allow a plugin to report which profiles it is to be active
  under.  If this (as in the default case) returns the empty set, it indicates that the plugin
  should be loaded under all profiles.  If it returns a non-empty set, it will only be loaded for
  those profiles that are specified by the importer/exporter class.
 */

/*!
  \fn QString QVersitOrganizerHandlerFactory::name() const
  This function should return a unique string that identifies the handlers provided by this factory.
  Typically, this will be of the form "com.nokia.qt.mobility.versit.samplehandler" with the
  appropriate domain and handler name substituted.
 */

/*!
  \fn int QVersitOrganizerHandlerFactory::index() const
  This function should return an index that helps with determining the order in which to run the
  plugins.  Plugins are run in the following order:
  \list
  \o Positively-indexed, ascending
  \o Zero-indexed
  \o Negatively-indexed, ascending
  \endlist
  For example, plugins with an index of 1 are run first and plugins of index -1 are run last.
  If more than one plugin share an index, the order of execution between them is undefined.

  By default, this returns 0, which is recommended for plugins with no special ordering
  requirements.
 */

/*!
  \fn QVersitOrganizerHandlerFactory::createHandler() const
  This function is called by the Versit importer or exporter class to create an instance of the
  handler provided by this factory.
 */

/*!
  \fn QVersitOrganizerHandler::~QVersitOrganizerHandler()
  Destroys this QVersitOrganizerHandler.
 */
