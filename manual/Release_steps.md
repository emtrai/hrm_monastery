1. Pull latest code from github, using Githup desktop
2. Open QT with latest code
3. Clean up project
4. Build new project
5. Update version in hrm_monastery.pro
```
DEFINES += VER_MAJOR=0
DEFINES += VER_MINOR=1
DEFINES += VER_PATCH=1
```
6. Disable log macro
```
# DEFINES += TEST_ENABLE
# DEFINES += LOG_LEVEL=4
# DEFINES += DEBUG_TRACE
# DEFINES += DEBUG_LOG
```
7. Build > Deploy
8. Copy file Quanlyhoidong.exe in dir build-xxx/release for release
9. Update release note, usermanual document if need