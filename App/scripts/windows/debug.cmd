@echo off

cd ../..

cmd /k .\node_modules\.bin\gulp --cwd %cd% --gulpfile scripts/gulpfile.js Debug

pause >nul