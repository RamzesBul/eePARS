@echo off

set MBEDTLS_LIB_FOLDER="back\mbedtls\"
set EXTERNAL_FOLDER="include\external_projects"

cd /d "%~dp0"
for %%i in (%MBEDTLS_LIB_FOLDER%"libmbedcrypto.a", %MBEDTLS_LIB_FOLDER%"libmbedtls.a", %MBEDTLS_LIB_FOLDER%"libmbedx509.a"
) do if not exist "%%~i" echo\File "%%~i" does Not exist && goto %:^(

:: Run more commands below, because all your files exist...
echo The files exist, you don't need to do anything.

goto :begin

%:^(
:: Run more commands below, because one some of your files don't exist
echo The files don't exist, I start cloning Mbed TLS and build it up.

if not exist "back\mbedtls\" (
    cd back\
    git clone -b v2.28.6 https://github.com/Mbed-TLS/mbedtls.git
    cd mbedtls\
    cmake -S . -B build\ -G "MinGW Makefiles"
    cmake --build build\ --target mbedtls --config Release
    mkdir ..\lib\mbedtls\
    copy build\library\*.a ..\lib\mbedtls\
    mkdir ..\%EXTERNAL_FOLDER%\mbedtls\
    mkdir ..\%EXTERNAL_FOLDER%\psa\
    copy build\include\mbedtls\*.h ..\%EXTERNAL_FOLDER%\mbedtls\
    copy build\include\psa\*.h ..\%EXTERNAL_FOLDER%\psa\

    cd ../
    rd /s /q mbedtls
    cd ../
) else (
    echo mbedtls already exist.
)

:begin

:: Build docker image
docker image build . -t eepars