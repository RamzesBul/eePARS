@echo off

:: Move to the directory where the script is located.
:: Move to the directory where the script is located.
cd /d "%~dp0"

:: Check if the required files exist.
for %%f in (
    "libmbedcrypto.a"
    "libmbedtls.a"
    "libmbedx509.a"
) do (
    if not exist "back\lib\mbedtls\%%~f" (
        echo File "back\lib\mbedtls\%%~f" does not exist.
        echo We need to clone Mbed TLS repository and build it up. Please wait.
        goto clone_mbedtls
    )
)

:: If all required files exist, we start building the project.
echo All required files exist. We start building the project.
goto :check_ipee


:clone_mbedtls

:: Create the folder for the Mbed TLS library.
set MBEDTLS_LIB_FOLDER=.\back\mbedtls
if not exist "%MBEDTLS_LIB_FOLDER%" (
    mkdir "%MBEDTLS_LIB_FOLDER%"
)

:: Clone the Mbed TLS repository.
echo Cloning Mbed TLS repository.
cd %MBEDTLS_LIB_FOLDER%
git clone -b v2.28.6 https://github.com/Mbed-TLS/mbedtls.git
cd mbedtls

:: Build the Mbed TLS library.
echo Building Mbed TLS.
cmake -S . -B build\ -G "MinGW Makefiles"
cmake --build build\ --target mbedtls --config Release

:: Copy the built libraries and headers.
set EXTERNAL_FOLDER=.\include\external_projects
echo Copying built libraries and headers.
mkdir ..\..\%EXTERNAL_FOLDER%\mbedtls
copy build\include\mbedtls\*.h ..\..\%EXTERNAL_FOLDER%\mbedtls
mkdir ..\..\%EXTERNAL_FOLDER%\psa
copy build\include\psa\*.h ..\..\%EXTERNAL_FOLDER%\psa
mkdir ..\..\lib\mbedtls
copy build\library\*.a ..\..\lib\mbedtls

:: Clean up the source folder.
echo Deleting Mbed TLS source folder.
cd ..\..\..
rmdir /s /q %MBEDTLS_LIB_FOLDER%


:check_ipee

:: Check if the required files exist.
if exist "lib\IPee\libIpEe.a" (
    goto build
)
for %%f in (
    "libIpEeDictionary.a"
    "libIpEeContainer.a"
) do (
    if not exist "lib\IPee\%%~f" (
        echo File "lib\IPee\%%~f" does not exist.
        echo We need to clone IPEE repository and build it up. Please wait.
        goto clone_ipee
    )
)

goto build


:clone_ipee

:: Create the folder for the IPEE library.
set IPEE_LIB_FOLDER=.\back\ipee
if not exist "%IPEE_LIB_FOLDER%" (
    mkdir "%IPEE_LIB_FOLDER%"
)

:: Clone the IPEE repository.
echo Cloning IPEE repository.
cd %IPEE_LIB_FOLDER%
git clone -b v1.0 https://github.com/cMeWHou/IPee.git
cd IPee

:: Build the IPEE library.
echo Building IPEE.
cmake -S . -B build\ -G "MinGW Makefiles"
cmake --build build\ --target all --config Release

:: Copy the built libraries and headers.
set INTERNAL_FOLDER=.\include\internal_projects
echo Copying built libraries and headers.
mkdir ..\..\%INTERNAL_FOLDER%\
copy include\dictionary.h ..\..\%INTERNAL_FOLDER%\
copy include\container.h ..\..\%INTERNAL_FOLDER%\
copy include\macro.h ..\..\%INTERNAL_FOLDER%\
mkdir ..\..\back\lib\IPee
copy build\libIpEeDictionary.a ..\..\lib\IPee
copy build\libIpEeContainer.a ..\..\lib\IPee

:: Clean up the source folder.
echo Deleting IPee source folder.
cd ..\..\..
rmdir /s /q "%IPEE_LIB_FOLDER%"

:build

:: Build the project.
:: docker image build . -t eepars