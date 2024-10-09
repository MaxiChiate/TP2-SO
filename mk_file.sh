#!/bin/bash

NOMBRE=TP2-SO

# Function to compile the project
compile() {
    # Check if a container with the same name exists
    if [ "$(docker ps -aq -f name=$NOMBRE)" ]; then
        echo "Removing existing container $NOMBRE."
        docker rm -f $NOMBRE
    fi

    # Create a new container
    echo "Creating a new container $NOMBRE."
    sudo docker run -d -v "$PWD":/root --security-opt seccomp:unconfined -ti --name $NOMBRE agodio/itba-so-multi-platform:3.0

    # Start the compilation process
    docker start $NOMBRE
    docker exec -it $NOMBRE make clean -C/root/Toolchain
    docker exec -it $NOMBRE make clean -C/root/
    docker exec -it $NOMBRE make -C/root/Toolchain
    docker exec -it $NOMBRE make -C/root/
    docker stop $NOMBRE
}

# Function to run the project
run() {
    # Set FLAG based on the input argument for debug mode
    FLAG=""
    [[ "$1" = "-d" ]] && FLAG="-s -S"

    # Set common qemu options
    QEMU_CMD="qemu-system-x86_64 $FLAG -hda Image/x64BareBonesImage.qcow2 -m 512"

    if [[ "$(uname)" == "Linux" ]]; then
        # Linux specific execution
        sudo $QEMU_CMD
    else
        # macOS specific execution
        export AUDIO_DRIVER="coreaudio"
        $QEMU_CMD -audiodev $AUDIO_DRIVER,id=audio0 -machine pcspk-audiodev=audio0
    fi
}


# Main script:


if  [ $# -eq 0 ]
then
      
        echo "Compiling..."
        compile

else

  case "$1" in
    -w)
        echo "Compiling and only showing warnings..."
        WARNINGS=$(compile | egrep "warning")
        echo "$WARNINGS"
        echo "Number of warnings: $(echo "$WARNINGS" | wc -l)"
        ;;

    -e)
        echo "Compiling and only showing errors..."
        ERRORS=$(compile | egrep "error")
        echo "$ERRORS"
        echo "Number of errors: $(echo "$ERRORS" | wc -l)"
        ;;

    -a)
        echo "Compiling and only showing errors and warnings..."
        OUTPUT=$(compile | egrep "error|warning")
        echo "$OUTPUT"
        echo "Number of warnings: $(echo "$OUTPUT" | egrep "warning" | wc -l)"
        echo "Number of errors: $(echo "$OUTPUT" | egrep "error" | wc -l)"
        ;;

    -r)
        echo "Compiling and running the project..."
        compile
        run
        ;;
    -d)
        echo "Compiling and running in debug mode..."
        compile
        run "-d"
        ;;
    *)
        echo "Usage: $0 [-w for warnings] [-r to run] [-d for debug mode]"
        ;;
  esac
fi
