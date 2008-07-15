/**
 * Copyright (c) 2008 Jeff Backus.
 *
 * Licensed under the GNU General Public License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Additionally, this code makes heavy use of the Qt libraries from
 * Trolltech.  These libraries are released under the GNU
 * General Public License, Version 2.0, as well.  Please contact
 * Trolltech for more information.
 *
 *     http://trolltech.com/
 * 
 */

/**
 * This handles running the screensaver and setting the
 * timer if necessary.
 */

#ifndef __SAVERRUNNER_H__
#define __SAVERRUNNER_H__

#include <QObject>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QProcess>
#include <QThread>

#include "ConfigManager.h"

class SaverRunner : public QThread {

	Q_OBJECT

public:
	/**
	 * Takes a reference to the config manager and then starts
	 * the screensaver process.  Every time the timer goes off
	 * the screen saver is terminated and another is started.
	 */
	void startSCR(ConfigManager* configMgr, QStringList args);

	/**
	 * Whether or not the screensaver is currently running.
	 */
	bool isRunning();

	/**
	 * This is the exit code of the screensaver.  If the
	 * timer interrupted the screensaver, the return value will
	 * be -1.
	 */
	int saverExitCode();

	void run();

public slots:

	/**
	 * Called when the screensaver QProcess has finished.
	 */
	void scrFinished(int exitCode, QProcess::ExitStatus exitStatus);

	/**
	 * Called when the screen saver QProcess has started.
	 */
	void scrStarted();

	/**
	 * Called by the timer once it has elapsed.
	 */
	void timerElapsed();

signals:
	/*
	 * Tells the screensaver to quit.
	 */
	//void stopScr();

	/**
	 * Emitted when the screen saver is changing.
	 */
	void changing();

	/**
	 * Emitted when finished.
	 */
	void done();

private:
	/**
	 * Starts a new screensaver in the QProcess.
	 */
	void startNewSCR();

	bool _bRunning;
	int _exitCode;
	bool _bTimerElapsed;
	QTimer* _timer;
	QProcess* _process;
	ConfigManager* _configMgr;
	QStringList _scrArgs;
	long _timerMillis;
};

#endif