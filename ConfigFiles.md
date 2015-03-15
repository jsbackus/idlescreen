# Introduction #

All of the programs that are part of the Idle Screen project store their configuration information within XML files.  On Windows systems, the per-user configuration files are stored in the idlescreen folder under Applications Data under the user's directory in the "Documents and Settings" directory.  Each program has its own directory, and within each
directory is a settings.xml file.  For example:
```
C:\Documents and Settings\awesomeuser\Application Data\idlescreen\PlasmaLenz
```
On `*`NIX based systems, the configuration files are stored in ~/.idlescreen/, with each program having its own directory with a settings.xml file inside.

The system defaults configuration files for Windows are stored in the Defaults directory where Idle Screen was installed.  For `*`NIX systems, the are stored in /etc/idlescreen/.  There will be an XML file for each program with the name _programname_.xml.

If the program cannot find a configuration file in either of these two places, it will resort to a set of defaults.