@echo off

:: Move to the directory where the script is located.
cd /d "%~dp0"

set MBEDTLS_LIB_FOLDER=.\back\mbedtls
set EXTERNAL_FOLDER=.\include\external_projects

:: Check if the required files exist.
for %%f in (
    "libmbedcrypto.a"
    "libmbedtls.a"
    "libmbedx509.a"
) do (
    if not exist "%MBEDTLS_LIB_FOLDER%\%%~f" (
        echo File "%MBEDTLS_LIB_FOLDER%\%%~f" does not exist.
        echo We need to clone Mbed TLS repository and build it up. Please wait.
        goto Clone_MbedTLS
    )
)

:: If all required files exist, we start building the project.
echo All required files exist. We start building the project.
goto :begin


:Clone_MbedTLS

:: Create the folder for the Mbed TLS library.
if not exist "%MBEDTLS_LIB_FOLDER%" (
    mkdir "%MBEDTLS_LIB_FOLDER%"
)

:: Clone the Mbed TLS repository.
echo Cloning Mbed TLS repository.
cd %MBEDTLS_LIB_FOLDER%
git clone -b v2.28.6 https://github.com/Mbed-TLS/mbedtls.git --recurse-submodules
cd mbedtls

:: Build the Mbed TLS library.
echo Building Mbed TLS.
cmake -S . -B build\ -G "MinGW Makefiles"
cmake --build build\ --target mbedtls --config Release

:: Copy the built libraries and headers.
echo Copying built libraries and headers.
mkdir ..\..\%EXTERNAL_FOLDER%\mbedtls
copy build\include\mbedtls\*.h ..\..\%EXTERNAL_FOLDER%\mbedtls
mkdir ..\..\%EXTERNAL_FOLDER%\psa
copy build\include\psa\*.h ..\..\%EXTERNAL_FOLDER%\psa
mkdir ..\..\lib\mbedtls
copy build\library\*.a ..\..\lib\mbedtls

:: Clean up the source folder.
echo Deleting Mbed TLS source folder.
cd ..\..
rmdir /s /q mbedtls


:begin

:: Build the project.
:: docker image build . -t eepars