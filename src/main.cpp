/****************************************************************************
**
** Copyright (C) 2007~2025 Colin Willcocks.
** Copyright (C) 2005~2007 Uco Mesdag.
** All rights reserved.
** This file is part of "SY-1000 FloorBoard".
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License along
** with this program; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
**
****************************************************************************/

// === CRASH LOG SYSTEM ===
// BEST PRACTICE: All crash logging uses only POSIX primitives so it is safe
// to call BEFORE QApplication is constructed, AND from inside signal handlers
// (where C++ runtime and Qt heap allocations are unsafe).
//
// Log file: ~/Library/Application Support/Gumtown/SY-1000FloorBoard/startup.log
// Prev log: ~/Library/Application Support/Gumtown/SY-1000FloorBoard/startup.log.prev
//
// To read after a crash:
//   cat ~/Library/Application\ Support/Gumtown/SY-1000FloorBoard/startup.log
//   cat ~/Library/Application\ Support/Gumtown/SY-1000FloorBoard/startup.log.prev

#include <execinfo.h>   // backtrace(), backtrace_symbols_fd()
#include <signal.h>     // signal(), SIGABRT etc.
#include <unistd.h>     // write(), open(), close()
#include <fcntl.h>      // O_WRONLY, O_APPEND, O_CREAT
#include <sys/stat.h>   // mkdir()
#include <ctime>        // time(), localtime(), strftime()
#include <cstring>      // strlen()
#include <cstdio>       // snprintf()
#include <exception>    // std::set_terminate
#include <cstdlib>      // atexit(), getenv()
#include <limits.h>     // PATH_MAX
#include <string>

#include <QApplication>
#include <QStyleFactory>
#include <QFontDatabase>
#include "mainWindow.h"
#include "Preferences.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include "customSplashScreen.h"

// ---------------------------------------------------------------------------
//  Crash log global state — plain C types, safe before Qt init
// ---------------------------------------------------------------------------
static char g_logFilePath[1024] = {0};

// Write a timestamped line to the log.  Async-signal-safe: uses only
// open/write/close (no malloc, no C++ runtime, no Qt).
static void writeRawLog(const char *msg)
{
    if (!g_logFilePath[0]) return;
    int fd = open(g_logFilePath, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd < 0) return;

    time_t now = time(nullptr);
    char timebuf[32];
    struct tm *tm_info = localtime(&now);
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S ", tm_info);
    char pidbuf[64];
    snprintf(pidbuf, sizeof(pidbuf), "[pid:%d] ", int(getpid()));

    write(fd, timebuf, strlen(timebuf));
    write(fd, pidbuf, strlen(pidbuf));
    write(fd, msg, strlen(msg));
    write(fd, "\n", 1);
    close(fd);
}

static void writeRawLogFmt(const char *fmt, const char *a, const char *b = "")
{
    char line[2048];
    snprintf(line, sizeof(line), fmt, a, b);
    writeRawLog(line);
}

static bool pathExists(const char *path)
{
    struct stat st;
    return (path && path[0] && stat(path, &st) == 0);
}

static std::string shellEscapeDoubleQuoted(const std::string &input)
{
    std::string out;
    out.reserve(input.size() + 8);
    for (char c : input) {
        if (c == '"' || c == '\\') {
            out.push_back('\\');
        }
        out.push_back(c);
    }
    return out;
}

// Detect the Qt runtime flavor the executable is linked against so we can
// choose a matching plugin path and avoid loading two Qt trees at once.
static bool executableLinksToNeedle(const char *exePath, const char *needle)
{
    if (!exePath || !exePath[0] || !needle || !needle[0]) {
        return false;
    }

    const std::string cmd = std::string("otool -L \"") +
                            shellEscapeDoubleQuoted(exePath) +
                            "\" 2>/dev/null";
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return false;
    }

    bool found = false;
    char line[2048];
    while (fgets(line, sizeof(line), pipe)) {
        if (strstr(line, needle)) {
            found = true;
            break;
        }
    }
    pclose(pipe);
    return found;
}

// Force deterministic Qt plugin loading from this app bundle to avoid
// environment/path bleed from Homebrew or other Qt installs.
static void configureQtPluginPaths(const char *argv0)
{
    if (!argv0 || !argv0[0]) {
        writeRawLog("STEP 1a: argv[0] unavailable; cannot preconfigure Qt plugin paths.");
        return;
    }

    char exePath[PATH_MAX] = {0};
    if (!realpath(argv0, exePath)) {
        writeRawLogFmt("STEP 1a: realpath failed for argv[0]=%s", argv0);
        return;
    }
    writeRawLogFmt("STEP 1a: executable path: %s", exePath);

    std::string exe(exePath);
    const std::string marker = "/Contents/MacOS/";
    const std::size_t markerPos = exe.find(marker);
    if (markerPos == std::string::npos) {
        writeRawLog("STEP 1b: not running from an app bundle path; keeping ambient Qt plugin paths.");
        return;
    }

    const bool linksHomebrewQt =
        executableLinksToNeedle(exePath, "/opt/homebrew/opt/qtbase/lib/QtCore.framework");
    writeRawLogFmt("STEP 1b: linksHomebrewQt=%s", linksHomebrewQt ? "true" : "false");

    if (linksHomebrewQt) {
        const std::string hbPluginsA = "/opt/homebrew/opt/qtbase/share/qt/plugins";
        const std::string hbPlatformsA = hbPluginsA + "/platforms";
        const std::string hbCocoaA = hbPlatformsA + "/libqcocoa.dylib";
        const std::string hbPluginsB = "/opt/homebrew/opt/qtbase/plugins";
        const std::string hbPlatformsB = hbPluginsB + "/platforms";
        const std::string hbCocoaB = hbPlatformsB + "/libqcocoa.dylib";

        if (pathExists(hbCocoaA.c_str()) || pathExists(hbCocoaB.c_str())) {
            const std::string pluginsDir = pathExists(hbCocoaA.c_str()) ? hbPluginsA : hbPluginsB;
            const std::string platformsDir = pathExists(hbCocoaA.c_str()) ? hbPlatformsA : hbPlatformsB;

            setenv("QT_PLUGIN_PATH", pluginsDir.c_str(), 1);
            setenv("QT_QPA_PLATFORM_PLUGIN_PATH", platformsDir.c_str(), 1);
            setenv("QT_QPA_PLATFORM", "cocoa", 1);

            writeRawLogFmt("STEP 1c: using Homebrew Qt plugins at %s", pluginsDir.c_str());
            writeRawLogFmt("STEP 1d: QT_PLUGIN_PATH=%s", pluginsDir.c_str());
            writeRawLogFmt("STEP 1e: QT_QPA_PLATFORM_PLUGIN_PATH=%s", platformsDir.c_str());
            writeRawLog("STEP 1f: QT_QPA_PLATFORM=cocoa");
            return;
        }

        writeRawLog("STEP 1c: Homebrew Qt detected but platform plugin not found; falling back to bundled plugins.");
    }

    const std::string contentsDir = exe.substr(0, markerPos) + "/Contents";
    const std::string pluginsDir = contentsDir + "/PlugIns";
    const std::string platformsDir = pluginsDir + "/platforms";
    const std::string cocoaPlugin = platformsDir + "/libqcocoa.dylib";

    if (!pathExists(cocoaPlugin.c_str())) {
        writeRawLogFmt("STEP 1c: bundled cocoa plugin missing: %s", cocoaPlugin.c_str());
        return;
    }

    setenv("QT_PLUGIN_PATH", pluginsDir.c_str(), 1);
    setenv("QT_QPA_PLATFORM_PLUGIN_PATH", platformsDir.c_str(), 1);
    setenv("QT_QPA_PLATFORM", "cocoa", 1);

    writeRawLogFmt("STEP 1d: QT_PLUGIN_PATH=%s", pluginsDir.c_str());
    writeRawLogFmt("STEP 1e: QT_QPA_PLATFORM_PLUGIN_PATH=%s", platformsDir.c_str());
    writeRawLog("STEP 1f: QT_QPA_PLATFORM=cocoa");
}

// Signal handler — called only on fatal crash-class signals.
// Writes signal name + stack trace to log then re-raises for core dump / OS dialog.
static void crashSignalHandler(int sig)
{
    const char *signame;
    switch (sig) {
        case SIGABRT: signame = "SIGABRT  (Abort — often a Qt fatal error or assert)"; break;
        case SIGSEGV: signame = "SIGSEGV  (Segmentation Fault — null/bad pointer)";   break;
        case SIGBUS:  signame = "SIGBUS   (Bus Error — misaligned memory access)";     break;
        case SIGFPE:  signame = "SIGFPE   (Floating-Point Exception / divide-by-zero)";break;
        case SIGILL:  signame = "SIGILL   (Illegal Instruction — corrupt binary?)";    break;
        default:      signame = "UNKNOWN SIGNAL";                                       break;
    }

    writeRawLog("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    writeRawLog("!!! CRASH DETECTED !!!");
    writeRawLog(signame);
    writeRawLog("Stack trace (most recent call last):");

    // backtrace_symbols_fd is async-signal-safe; backtrace() itself may malloc
    // but is the best option available on Darwin without a dedicated unwinder.
    void *array[64];
    int size = backtrace(array, 64);
    int fd = open(g_logFilePath, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd >= 0) {
        backtrace_symbols_fd(array, size, fd);
        write(fd, "\n", 1);
        close(fd);
    }

    writeRawLog("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

    // Restore default handler and re-raise so macOS crash reporter still fires.
    signal(sig, SIG_DFL);
    raise(sig);
}

static void onProcessExit()
{
    writeRawLog("STEP EXIT: atexit() reached.");
}

static void terminateHandler()
{
    writeRawLog("!!! std::terminate() invoked !!!");
    try {
        std::exception_ptr eptr = std::current_exception();
        if (eptr) {
            std::rethrow_exception(eptr);
        }
        writeRawLog("terminate reason: no active exception");
    } catch (const std::exception &e) {
        writeRawLogFmt("terminate reason: %s", e.what());
    } catch (...) {
        writeRawLog("terminate reason: unknown non-std exception");
    }
    abort();
}

// Qt message handler — captures Qt's own qFatal/qCritical/qWarning output.
// MUST be installed via qInstallMessageHandler BEFORE QApplication is created
// to catch errors during platform plugin loading.
static void qtLogHandler(QtMsgType type, const QMessageLogContext &ctx, const QString &msg)
{
    const char *typeStr;
    switch (type) {
        case QtDebugMsg:    typeStr = "Qt DEBUG";    break;
        case QtInfoMsg:     typeStr = "Qt INFO";     break;
        case QtWarningMsg:  typeStr = "Qt WARNING";  break;
        case QtCriticalMsg: typeStr = "Qt CRITICAL"; break;
        case QtFatalMsg:    typeStr = "Qt FATAL";    break;
        default:            typeStr = "Qt UNKNOWN";  break;
    }

    // Build a single fixed-buffer record so startup/cause logs stay scan-friendly.
    char line[4096];
    QByteArray ba = msg.toUtf8();
    const char *file = (ctx.file && ctx.file[0]) ? ctx.file : "?";
    const char *func = (ctx.function && ctx.function[0]) ? ctx.function : "?";
    snprintf(line, sizeof(line), "[%s] %s:%d %s | %s", typeStr, file, ctx.line, func, ba.constData());
    writeRawLog(line);

    if (type == QtFatalMsg) {
        const char *qtPluginPath = getenv("QT_PLUGIN_PATH");
        const char *qtPlatformPluginPath = getenv("QT_QPA_PLATFORM_PLUGIN_PATH");
        const char *qtPlatform = getenv("QT_QPA_PLATFORM");
        writeRawLogFmt("FATAL ENV: QT_PLUGIN_PATH=%s", qtPluginPath ? qtPluginPath : "<unset>");
        writeRawLogFmt("FATAL ENV: QT_QPA_PLATFORM_PLUGIN_PATH=%s", qtPlatformPluginPath ? qtPlatformPluginPath : "<unset>");
        writeRawLogFmt("FATAL ENV: QT_QPA_PLATFORM=%s", qtPlatform ? qtPlatform : "<unset>");

        // Let our signal handler add the stack trace before the process dies.
        abort();
    }
}

// Initialise the crash log system.  Call this as the very first line of main()
// — before QApplication, before any Qt code.
static void initCrashLog()
{
    const char *home = getenv("HOME");
    if (!home || !home[0]) home = "/tmp";

    // Ensure the full directory chain exists.
    char tmp[768];
    snprintf(tmp, sizeof(tmp), "%s/Library", home);                             mkdir(tmp, 0755);
    snprintf(tmp, sizeof(tmp), "%s/Library/Application Support", home);         mkdir(tmp, 0755);
    snprintf(tmp, sizeof(tmp), "%s/Library/Application Support/Gumtown", home); mkdir(tmp, 0755);

    char dirPath[768];
    snprintf(dirPath, sizeof(dirPath),
             "%s/Library/Application Support/Gumtown/SY-1000FloorBoard", home);
    mkdir(dirPath, 0755);

    // Rotate: move previous startup.log -> startup.log.prev
    char prevPath[1024];
    snprintf(g_logFilePath, sizeof(g_logFilePath), "%s/startup.log",      dirPath);
    snprintf(prevPath,      sizeof(prevPath),       "%s/startup.log.prev", dirPath);
    rename(g_logFilePath, prevPath);   // OK if file does not exist yet

    // Write header
    writeRawLog("=======================================================");
    writeRawLog("  SY-1000FloorBoard Startup Log");
    writeRawLog("  Version: 2026.03.05");
    writeRawLog("=======================================================");
    writeRawLog("STEP 0: Crash log initialised.");
    {
        char info[2048];
        char cwd[1024] = {0};
        if (!getcwd(cwd, sizeof(cwd))) {
            snprintf(cwd, sizeof(cwd), "<unavailable>");
        }
        snprintf(info, sizeof(info), "LAUNCH CONTEXT: pid=%d cwd=%s", int(getpid()), cwd);
        writeRawLog(info);
    }

    // Install POSIX signal handlers
    signal(SIGABRT, crashSignalHandler);
    signal(SIGSEGV, crashSignalHandler);
    signal(SIGBUS,  crashSignalHandler);
    signal(SIGFPE,  crashSignalHandler);
    signal(SIGILL,  crashSignalHandler);
    // Do not trap SIGTERM as a crash. Users closing the app should not be
    // reported as "CRASH DETECTED" in startup.log.

    // Install Qt message handler (must be before QApplication construction)
    qInstallMessageHandler(qtLogHandler);
    std::set_terminate(terminateHandler);
    atexit(onProcessExit);

    writeRawLog("STEP 0b: Signal handlers and Qt message handler installed.");
}
// === END CRASH LOG SYSTEM ===


// ---------------------------------------------------------------------------
//  main()
// ---------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    // *** ALWAYS FIRST — captures crashes that occur during QApplication init ***
    initCrashLog();

    configureQtPluginPaths((argc > 0) ? argv[0] : nullptr);
    writeRawLog("STEP 1: About to construct QApplication...");
    //qputenv ("QT_AUTO_SCREEN_SCALE_FACTOR", QByteArray("1"));

    // CRASH FIX (macOS 26+ / Tahoe): Qt 5's libqmacstyle.dylib crashes during
    // QApplicationPrivate::initialize() on macOS 26.3 because the macOS native
    // style plugin accesses APIs that were removed.  Force Fusion style BEFORE
    // QApplication is constructed so Qt never loads libqmacstyle.dylib at all.
    setenv("QT_STYLE_OVERRIDE", "fusion", 1);

    QApplication app(argc, argv);
    writeRawLog("STEP 2: QApplication constructed OK.");

    app.setOrganizationName("Gumtown");
    app.setApplicationName("SY-1000FloorBoard");
    //app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    // Load Boss-reference Roboto Condensed fonts from embedded resources.
    QFontDatabase::addApplicationFont(":/fonts/RobotoCondensed-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/RobotoCondensed-Regular.ttf");
    writeRawLog("STEP 2b: Roboto Condensed fonts loaded.");

    writeRawLog("STEP 3: App name/org set.");

#ifdef Q_OS_ANDROID
   AndroidAskReadWriteRights(); // before creating the log file

   QDir().setCurrent(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
   QDir().setCurrent("./SY1000FloorBoard");
#endif

#ifdef Q_OS_WIN
    QDir().setCurrent(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
#endif

#ifdef Q_OS_IOS
    QDir().setCurrent(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    QDir().setCurrent("./SY1000FloorBoard");
#endif

    // NOTE: On macOS we do NOT chdir — the Preferences class already resolves
    // preferences.xml to ~/Library/Application Support/Gumtown/SY-1000FloorBoard/
    // via QStandardPaths::AppDataLocation.  The old chdir to ../Shared/SY1000FloorBoard
    // pointed to a non-existent path and caused silent write failures.

    writeRawLog("STEP 4: About to load Preferences...");

    bool ok;
    Preferences *preferences = Preferences::Instance(); // Load the preferences.
    preferences->loadPreferences(":preferences.xml.dist");

    QString buildVersion = preferences->getPreferences("General", "Application", "version");

    // BUG FIX: The Preferences singleton resolves all paths via QStandardPaths.
    // Load dist defaults first, then overlay the user file (if it exists and
    // matches the current build version).
    preferences->loadPreferences(":preferences.xml.dist");
    preferences->loadPreferences("preferences.xml");   // resolved inside Preferences to AppDataLocation
    if (preferences->getPreferences("General", "Application", "version") != buildVersion)
    {
        preferences->loadPreferences(":preferences.xml.dist");
    }

    writeRawLog("STEP 5: Preferences loaded.");

    QString setting = preferences->getPreferences("Scheme", "Style", "select");
    int choice = setting.toInt(&ok, 16);
    QString style;
    if      (choice == 3) { style = "motif";      }
    else if (choice == 2) { style = "cleanlooks";  }
    else if (choice == 1) { style = "plastique";   }
    else                  { style = "Fusion";       }

#if defined(Q_OS_WIN)
    if (style == "motif")      { style = "windows";     }
    if (style == "cleanlooks") { style = "windowsxp";   }
    if (style == "plastique")  { style = "Windowsvista";}
#endif

#if defined(Q_OS_MAC)
    // BUG FIX: "windows" and "cde" are not valid Qt style names on macOS.
    // Fall back to Fusion (cross-platform) for unsupported legacy style choices.
    // "plastique" -> "macintosh" is valid and kept.
    if (style == "motif")      { style = "Fusion";    }   // was "windows" (invalid on macOS)
    if (style == "cleanlooks") { style = "Fusion";    }   // was "cde"     (invalid on macOS)
    if (style == "plastique")  { style = "macintosh"; }
#endif

    {
        char buf[256];
        snprintf(buf, sizeof(buf), "STEP 6: Setting Qt style: %s", style.toUtf8().constData());
        writeRawLog(buf);
    }
    app.setStyle(QStyleFactory::create(style));
    writeRawLog("STEP 6b: Qt style applied.");

    QString lang = preferences->getPreferences("Language", "Locale", "select");
    choice = lang.toInt(&ok, 16);
    // Loading translation
    /*QTranslator translator;

    if (choice == 6)     {translator.load(":language_ch.qm"); }
    else if (choice ==5) {translator.load(":language_jp.qm"); }
    else if (choice ==4) {translator.load(":language_es.qm"); }
    else if (choice ==3) {translator.load(":language_pt.qm"); }
    else if (choice ==2) {translator.load(":language_ge.qm"); }
    else if (choice ==1) {translator.load(":language_fr.qm"); };

    app.installTranslator(&translator);*/


    int windowWidth;
    if (preferences->getPreferences("Window", "Collapsed", "width").isEmpty())
    {
        windowWidth = preferences->getPreferences("Window", "Collapsed", "defaultwidth").toInt(&ok, 10);
    }
    else
    {
        windowWidth = preferences->getPreferences("Window", "Collapsed", "width").toInt(&ok, 10);
    }

    QRect screen = QGuiApplication::primaryScreen()->availableGeometry();
    int screenWidth  = screen.width();
    int screenHeight = screen.height();
    double resolution = screenWidth;
    if (screenHeight > screenWidth) { resolution = screenHeight; }   // tablet portrait
    // LAYOUT FIX: Changed divisor from 1.2 → 1.07 so the window fills ~93 % of
    // screen width instead of ~83 %.  On a 1728-px screen with windowWidth=1350
    // this gives ratio ≈ 1.20 (was 1.07), making all blocks, text, and spacing
    // proportionally larger while still leaving a comfortable window margin.
    resolution = resolution / (windowWidth * 1.07);
    QString scaleRatio = QString::number(resolution, 'f', 2);
    double dpi = QGuiApplication::primaryScreen()->logicalDotsPerInch();
    dpi = (96 / dpi) * (resolution);   // ratio 1:1 at 96 dpi
#if defined(Q_OS_MAC)
    // BUG FIX: On macOS, logicalDotsPerInch() returns 72 (the classic Mac
    // point system) even on Retina displays — because macOS handles its own
    // HiDPI pixel doubling.  The (96/72) correction inflates fonts by 1.33×
    // beyond the window scale, pushing text out of every widget.
    // Fix: use the window scale ratio directly for fonts on macOS.
    dpi = resolution;
#endif
    QString dpiRatio = QString::number(dpi, 'f', 2);
    if (preferences->getPreferences("Window", "AutoScale", "bool") == "true")
    {
        preferences->setPreferences("Window", "Font",     "ratio", dpiRatio);
        preferences->setPreferences("Window", "Scale",    "ratio", scaleRatio);
        preferences->setPreferences("Window", "Position", "x",     "1");
        preferences->setPreferences("Window", "Position", "y",     "1");
        preferences->savePreferences();
    /*} else {
        const double scale = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
        dpi = dpi*(scale/1.6);
        dpiRatio = QString::number(dpi,'f',2);
        preferences->setPreferences("Window", "Font", "ratio", dpiRatio);
        preferences->savePreferences();*/
    }

    double ratio  = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    double fratio = preferences->getPreferences("Window", "Font",  "ratio").toDouble(&ok);
    // Global readability bump: keep layout scale unchanged but render text larger.
    // Store a base ratio so repeated launches do not compound the boost.
    // LAYOUT FIX: Reduced from 1.20 → 1.05 because the auto-scale divisor change
    // (1.2 → 1.07) already increases ratio from ~1.07 to ~1.20 on a 1728-px screen.
    // Stacking fontBoost=1.20 on top would give fratio=1.44, too large.
    const double fontBoost = 1.05;
    bool baseOk = false;
    double baseFratio = preferences->getPreferences("Window", "Font", "base_ratio").toDouble(&baseOk);
    if (!baseOk || baseFratio <= 0.0)
    {
        if (preferences->getPreferences("Window", "Font", "ui_boost") == "true")
        {
            baseFratio = fratio / fontBoost;
        }
        else
        {
            baseFratio = fratio;
        }
    }
    fratio = baseFratio * fontBoost;
    preferences->setPreferences("Window", "Font", "base_ratio", QString::number(baseFratio, 'f', 2));
    preferences->setPreferences("Window", "Font", "ratio", QString::number(fratio, 'f', 2));
    preferences->setPreferences("Window", "Font", "ui_boost", "true");
    QFont Wfont("Roboto Condensed", 10 * fratio);
    QApplication::setFont(Wfont);

    {
        char buf[256];
        snprintf(buf, sizeof(buf),
                 "STEP 7: scale=%.2f  font=%.2f  screen=%dx%d",
                 ratio, fratio, screenWidth, screenHeight);
        writeRawLog(buf);
    }

    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("*");
    sysxIO->deBug("*");
    sysxIO->deBug("*");
    sysxIO->deBug("*");
    sysxIO->deBug("*");
    sysxIO->deBug("*");
    sysxIO->deBug("*");
    sysxIO->deBug("*");
    sysxIO->deBug("*");
    sysxIO->deBug("*");
    sysxIO->deBug("*");
    sysxIO->deBug("*******************************************************");
    sysxIO->deBug("SY-1000FloorBoard INITIALISE !! - Scale Ratio = " + preferences->getPreferences("Window", "Scale", "ratio") + " - Font Ratio = " + preferences->getPreferences("Window", "Font", "ratio"));

    writeRawLog("STEP 8: SysxIO initialised.");

    // Splash Screen setup uses subclassed QSplashScreen for message position control.
    writeRawLog("STEP 9: Loading splash image...");
    QPixmap splashImage(":/images/splash.png");
    QPixmap splashMask(":/images/splashmask.png");
    splashImage = splashImage.scaled(429 * ratio, 269 * ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    splashMask  = splashMask.scaled( 429 * ratio, 269 * ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    customSplashScreen *splash = new customSplashScreen(splashImage);
    splash->setMessageRect(QRect(7 * ratio, 253 * ratio, 415 * ratio, 14 * ratio), Qt::AlignCenter);

    QFont splashFont;
    splashFont.setFamily("Roboto Condensed");
    splashFont.setBold(true);
    splashFont.setPixelSize(10 * fratio);
    splash->setFont(splashFont);
    //splash->setMask(splashMask);
    //splash->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::SplashScreen);

    QString mode = "Guitar Mode";
    if (preferences->getPreferences("Window", "BassMode", "bool") == "true")
    {
        mode = "Bass Mode";
    }
    if (preferences->getPreferences("Window", "Splash", "bool") == "true")
    {
        splash->show();
        splash->showStatusMessage(mode + QObject::tr("  Initializing.. please wait..."));
        splash->progressBar->setValue(20);
    }
    app.processEvents();
    writeRawLog("STEP 10: Splash shown.");

    writeRawLog("STEP 11: Constructing mainWindow...");
    mainWindow window;
    window.setFont(Wfont);
    writeRawLog("STEP 12: mainWindow constructed OK.");

    QObject::connect(&window, SIGNAL(closed()), &app, SLOT(quit()));

    splash->showStatusMessage(QObject::tr("Checking license file..."));
    splash->progressBar->setValue(60);
    app.processEvents();
    if (!QFile("license.txt").exists())
    {
        QFile file(":license.txt");
        file.copy("license.txt");
        file.close();
    }

    splash->showStatusMessage(QObject::tr("Loading midi mapping..."));
    splash->progressBar->setValue(70);
    app.processEvents();
    writeRawLog("STEP 13: Loading MIDI map...");
    MidiTable *midiTable = MidiTable::Instance();
    midiTable->loadMidiMap();
    writeRawLog("STEP 14: MIDI map loaded.");

    splash->showStatusMessage(QObject::tr("Initializing main window..."));
    splash->progressBar->setValue(80);
    app.processEvents();
    window.setWindowFlags(
                Qt::WindowTitleHint
                | Qt::WindowMinimizeButtonHint
                | Qt::WindowCloseButtonHint
                //| Qt::MSWindowsFixedSizeDialogHint
                );
    window.setWindowIcon(QIcon(":/images/windowicon.png"));

    QString x_str = preferences->getPreferences("Window", "Position", "x");
    QString y_str = preferences->getPreferences("Window", "Position", "y");

    int windowHeight;
    if (preferences->getPreferences("Window", "Collapsed", "bool") == "true" &&
        preferences->getPreferences("Window", "Restore",   "sidepanel") == "true")
    {
        if (preferences->getPreferences("Window", "Collapsed", "width").isEmpty())
        {
            windowWidth = preferences->getPreferences("Window", "Collapsed", "defaultwidth").toInt(&ok, 10);
        }
        else
        {
            windowWidth = preferences->getPreferences("Window", "Collapsed", "width").toInt(&ok, 10);
        }
    }
    else
    {
        if (preferences->getPreferences("Window", "Size", "width").isEmpty())
        {
            windowWidth = preferences->getPreferences("Window", "Size", "minwidth").toInt(&ok, 10);
        }
        else
        {
            windowWidth = preferences->getPreferences("Window", "Size", "width").toInt(&ok, 10);
        }
    }

    if (preferences->getPreferences("Window", "Restore", "window") == "true" && !x_str.isEmpty())
    {
        splash->showStatusMessage(QObject::tr("Restoring window position..."));
        app.processEvents();
        if (preferences->getPreferences("Window", "Size", "height").isEmpty())
        {
            windowHeight = preferences->getPreferences("Window", "Size", "minheight").toInt(&ok, 10);
        }
        else
        {
            windowHeight = preferences->getPreferences("Window", "Size", "minheight").toInt(&ok, 10);
        }

        if (windowHeight > 6600)  { windowHeight = 6600; }
        if (windowHeight < 369)   { windowHeight = 369;  }
        if (windowWidth  > 12300) { windowWidth  = 12300;}
        if (windowWidth  < 623)   { windowWidth  = 623;  }
        window.resize(QSize(windowWidth * ratio, windowHeight * ratio));
        window.move(x_str.toInt(&ok, 10), y_str.toInt(&ok, 10));
    }
    else
    {
        splash->showStatusMessage(QObject::tr("Centering main window..."));
        splash->progressBar->setValue(90);
        app.processEvents();
        QRect screen2 = QGuiApplication::primaryScreen()->availableGeometry();
        int sw = screen2.width();
        int sh = screen2.height();

        windowHeight = preferences->getPreferences("Window", "Size", "minheight").toInt(&ok, 10);

        int x = (sw - windowWidth)  / 2;
        int y = (sh - windowHeight) / 2;
        window.setGeometry(x, y, window.width(), window.height());
    }

    writeRawLog("STEP 15: Window geometry set.");

    splash->showStatusMessage(QObject::tr("Finished Initializing..."));
    splash->progressBar->setValue(100);
    app.processEvents();
    window.show();
    splash->finish(&window);

    const QByteArray debugCaptureDir = qgetenv("SY1000_DEBUG_CAPTURE_DIR");
    if(!debugCaptureDir.isEmpty())
    {
        const QString capturePath = QString::fromUtf8(debugCaptureDir);
        writeRawLogFmt("STEP 16a: debug capture dir=%s", capturePath.toUtf8().constData());
        QTimer::singleShot(1200, &window, [&window, &app, capturePath]()
        {
            window.exportDebugScreens(capturePath);
            app.quit();
        });
    }

    writeRawLog("STEP 16: Window shown. Entering event loop...");

    int result = app.exec();

    {
        char buf[64];
        snprintf(buf, sizeof(buf), "STEP 17: Event loop exited (code %d). Clean shutdown.", result);
        writeRawLog(buf);
    }

    return result;
}
