@echo off
echo Start deploying package v%1 to git...
echo:

cd ..\..\
git subtree split --prefix=Unity/UDlib/Assets/UDlib -b unitypack || GOTO RET
git tag %1 unitypack || GOTO RET
git push origin unitypack --tags || GOTO RET

echo:
echo [32m:) Package v%1 is successfully deployed![0m

:RET

IF %ERRORLEVEL% NEQ 0 (
	echo:
	echo [31m:( Failed to deploy package v%1![0m
)
echo:
PAUSE
EXIT %ERRORLEVEL%