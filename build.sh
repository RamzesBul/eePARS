#!/bin/bash

# Move to the directory where the script is located.
cd "$(dirname "$0")"

MBEDTLS_LIB_FOLDER=./back/mbedtls
EXTERNAL_FOLDER=./include/external_projects

# Function to clone MbedTLS if required files are missing.
function clone_mbedtls {
    # Create the folder for the Mbed TLS library if it doesn't exist.
    if [ ! -d "$MBEDTLS_LIB_FOLDER" ]; then
        mkdir "$MBEDTLS_LIB_FOLDER"
    fi

    # Clone the Mbed TLS repository.
    echo "Cloning Mbed TLS repository."
    cd "$MBEDTLS_LIB_FOLDER"
    git clone -b v2.28.6 https://github.com/Mbed-TLS/mbedtls.git
    cd mbedtls

    # Build the Mbed TLS library.
    echo "Building Mbed TLS."
    cmake -S . -B build/ -G "Unix Makefiles"
    cmake --build build/ --target mbedtls --config Release

    # Copy the built libraries and headers.
    echo "Copying built libraries and headers."
    mkdir -p "../../$EXTERNAL_FOLDER/mbedtls"
    cp build/include/mbedtls/*.h "../../$EXTERNAL_FOLDER/mbedtls"
    mkdir -p "../../$EXTERNAL_FOLDER/psa"
    cp build/include/psa/*.h "../../$EXTERNAL_FOLDER/psa"
    mkdir -p "../../lib/mbedtls"
    cp build/library/*.a "../../lib/mbedtls"

    # Clean up the source folder.
    echo "Deleting Mbed TLS source folder."
    cd ../..
    rm -rf mbedtls
}

# Check if the required files exist.
files_exist=true
for f in "libmbedcrypto.a" "libmbedtls.a" "libmbedx509.a"; do
    if [ ! -f "$MBEDTLS_LIB_FOLDER/$f" ]; then
        echo "File $MBEDTLS_LIB_FOLDER/$f does not exist."
        echo "We need to clone Mbed TLS repository and build it up. Please wait."
        files_exist=false
        break
    fi
done

# If required files don't exist, clone MbedTLS, else proceed with building.
if [ "$files_exist" = false ]; then
    clone_mbedtls
else
    echo "All required files exist. We start building the project."
fi

# Build the project.
# docker image build . -t eepars