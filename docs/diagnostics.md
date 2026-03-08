# SY-1000FloorBoard Diagnostics

This project now writes startup/crash diagnostics to:

- `~/Library/Application Support/Gumtown/SY-1000FloorBoard/startup.log`
- `~/Library/Application Support/Gumtown/SY-1000FloorBoard/startup.log.prev`
- macOS crash reports: `~/Library/Logs/DiagnosticReports/SY-1000FloorBoard-*.ips`

## Key Log Markers

- `STEP N`: startup milestones.
- `LAUNCH CONTEXT`: PID and working directory at startup.
- `APP_RESTART begin`: app requested a self-restart.
- `AUTOMODE_RESTART request`: auto-mode detected possible guitar/bass mismatch.
- `AUTOMODE_RESTART blocked`: restart-loop prevention activated.
- `STEP EXIT: atexit() reached.`: process exit callback ran.
- `!!! CRASH DETECTED !!!`: signal-based crash handler fired.
- `[Qt CRITICAL]` / `[Qt FATAL]`: high-severity Qt/runtime errors.

## Fast Triage Commands

```bash
# 1) Current startup/crash trace
cat ~/Library/Application\ Support/Gumtown/SY-1000FloorBoard/startup.log

# 2) Previous run (before last restart)
cat ~/Library/Application\ Support/Gumtown/SY-1000FloorBoard/startup.log.prev

# 3) Restart-loop and critical events only
rg -n "APP_RESTART|AUTOMODE_RESTART|Qt CRITICAL|Qt FATAL|CRASH DETECTED|STEP EXIT" \
  ~/Library/Application\ Support/Gumtown/SY-1000FloorBoard/startup.log \
  ~/Library/Application\ Support/Gumtown/SY-1000FloorBoard/startup.log.prev

# 4) Latest macOS crash report
ls -t ~/Library/Logs/DiagnosticReports/SY-1000FloorBoard-*.ips | head -n 1 | xargs sed -n '1,180p'
```

## Reading the Auto-Mode Loop Case

If you see:

- `AUTOMODE_RESTART request ... replyBytes=0`
- followed by `AUTOMODE_RESTART blocked ...`

then the app did not receive a valid SY-1000 bulk reply, tried one auto-restart,
and intentionally blocked additional restarts to prevent a relaunch loop.
This is usually a MIDI routing/device-mode mismatch, not a kernel crash.
