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

#include "SaverRunner.h"

/*
 * Takes a reference to the config manager and then starts
 * the screensaver process.  Every time the timer goes off
 * the screen saver is terminated and another is started.
 */
void SaverRunner::startSCR(ConfigManager* configMgr, QStringList args){
	_bTimerElapsed = false;
	_exitCode = -1;
	_bRunning = false;
	_timer = NULL;
	_process = NULL;
	_configMgr = configMgr;
	_scrArgs = args;
	_timerMillis = 0;

	if(_configMgr->useTimer()) {
		_timerMillis = ((long)_configMgr->getTimerMinutes())*60000;
	}

	//set up timer
	if(_timerMillis > 0) {
		_timer = new QTimer();
		connect(_timer, SIGNAL(timeout()), this, SLOT(timerElapsed()));
		_timer->setSingleShot(true);
	}
	

	//create QProcess
	_process = new QProcess();
	connect(_process, SIGNAL(started()), this, SLOT(scrStarted()));
	connect(_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(scrFinished(int, QProcess::ExitStatus)));
	
	startNewSCR();
}

/*
 * Whether or not the screensaver is currently running.
 */
bool SaverRunner::isRunning() {
	return _bRunning;
}

/*
 * This is the exit code of the screensaver.  If the
 * timer interrupted the screensaver, the return value will
 * be -1.
 */
int SaverRunner::saverExitCode() {
	if(_bTimerElapsed) {
		return -1;
	}
	return _exitCode;
}

/*
 * Called when the screensaver QProcess has finished.
 */
void SaverRunner::scrFinished(int exitCode, QProcess::ExitStatus exitStatus) {
	_bRunning = false;
	_exitCode = exitCode;

	//if exit code was 0 & the timer didn't go off,
	//finish, otherwise start a new one
	if(!_bTimerElapsed && _exitCode == 0) {
		//force timer to as soon as possible if it is valid
		if(_timer != NULL) {
			_timer->stop();
		}
		//save history
		if(_configMgr != NULL)
			_configMgr->save();
		//let the app manager know we're done
		emit done();
	} else {
		emit changing();
		startNewSCR();
	}
}

/*
 * Called when the screen saver QProcess has started.
 */
void SaverRunner::scrStarted() {
	//start timer if necessary
	if(_timer != NULL) {
		_timer->start(_timerMillis);
		_bTimerElapsed = false;
	}
	_bRunning = true;
}

/*
 * Called by the timer once it has elapsed.
 */
void SaverRunner::timerElapsed() {
	_bTimerElapsed = true;

	//stop the scr.
	if(_bRunning) {
		_process->terminate();
	}
}

/*
 * Starts a new screensaver in the QProcess.
 */
void SaverRunner::startNewSCR() {
	QString path = _configMgr->getNextScr();
	_configMgr->save();

	_process->start(path, _scrArgs);
}

void SaverRunner::run() {
	//start the message loop
	exec();
}
