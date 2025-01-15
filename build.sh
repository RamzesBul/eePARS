#!/bin/bash

# Move to the directory where the script is located.
cd "$(dirname "$0")"

MBEDTLS_LIB_FOLDER=./back/mbedtls
EXTERNAL_FOLDER=./include/external_projects

# Function to clone MbedTLS if required files are missing.
clone_mbedtls() {
    cd "$(dirname "$0")"

    # Create the folder for the Mbed TLS library if it doesn't exist.
    if [ ! -d "$MBEDTLS_LIB_FOLDER" ]; then
        mkdir "$MBEDTLS_LIB_FOLDER"
    fi

    # Clone the Mbed TLS repository.
    echo "Cloning Mbed TLS repository."
    cd "$MBEDTLS_LIB_FOLDER"
    git clone -b v3.6.2 https://github.com/Mbed-TLS/mbedtls.git
    cd mbedtls
    git submodule update --init

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
    cd "$(dirname "$0")"
    rm -rf "$MBEDTLS_LIB_FOLDER"
}

# Check if the required files exist.
mbedtls_exist=true
for f in "libmbedcrypto.a" "libmbedtls.a" "libmbedx509.a"; do
    if [ ! -f "back/lib/mbedtls/$f" ]; then
        echo "File back/lib/mbedtls/$f does not exist."
        echo "We need to clone Mbed TLS repository and build it up. Please wait."
        mbedtls_exist=false
        break
    fi
done

# If required files don't exist, clone MbedTLS, else proceed with building.
if [ "$mbedtls_exist" = false ]; then
    clone_mbedtls
else
    echo "All required files exist. We start building the project."
fi

IPEE_LIB_FOLDER=./back/ipee
INTERNAL_FOLDER=./include/internal_projects

# Function to clone IPEE if required files are missing.
clone_ipee() {
    cd "$(dirname "$0")"

    # Create the folder for the IPEE library if it doesn't exist.
    if [ ! -d "$IPEE_LIB_FOLDER" ]; then
        mkdir "$IPEE_LIB_FOLDER"
    fi

    # Clone the IPEE repository.
    echo "Cloning IPEE repository."
    cd "$IPEE_LIB_FOLDER"
    git clone -b v1.7 https://github.com/cMeWHou/IPee.git
    cd IPee

    # Build the IPEE library.
    echo "Building IPEE."
    cmake -S . -B build/ -G "Unix Makefiles"
    cmake --build build/ --target all --config Release

    # Copy the built libraries and headers.
    echo "Copying built libraries and headers."
    mkdir -p "../../$INTERNAL_FOLDER/"
    cp include/dictionary.h "../../$INTERNAL_FOLDER"
    cp include/container.h "../../$INTERNAL_FOLDER"
    cp include/threadpool.h "../../$INTERNAL_FOLDER"
    cp include/event.h "../../$INTERNAL_FOLDER"
    cp include/macro.h "../../$INTERNAL_FOLDER"
    mkdir -p "../../lib/IPee"
    cp build/libIpEeDictionary.a "../../lib/IPee"
    cp build/libIpEeContainer.a "../../lib/IPee"
    cp build/libIpEeEvent.a "../../lib/IPee"
    cp build/libIpEeThreadpool.a "../../lib/IPee"

    # Clean up the source folder.
    echo "Deleting IPEE source folder."
    cd "$(dirname "$0")"
    rm -rf "$IPEE_LIB_FOLDER"
}

# Check if the required files exist.
ipee_exist=true
if [ ! -f "lib/IPee/libIpEe.a" ]; then
    ipee_exist=false
fi
if [ "$ipee_exist" = false ]; then
    ipee_exist=true
    for f in "libIpEeDictionary.a" "libIpEeContainer.a" "libIpEeEvent.a" "libIpEeThreadpool.a"; do
        if [ ! -f "back/lib/IPee/$f" ]; then
            ipee_exist=false
            echo "File back/lib/IPee/$f does not exist."
            echo "We need to clone IPEE repository and build it up. Please wait."
            break
        fi
    done
fi

# If required files don't exist, clone IPEE, else proceed with building.
if [ "$ipee_exist" = false ]; then
    clone_ipee
else
    echo "All required files exist. We start building the project."
fi

# Build the project.
# docker image build . -t eepars