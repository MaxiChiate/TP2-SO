#!/bin/bash

NOMBRE=TP2-SO

# Function to create container

container() {

        if ! command -v docker &>/dev/null
    then
        echo -e "${RED}Docker is not installed or not in the PATH.${RESET}"
        exit 1
    fi

    # Check if the container exists
    if [ "$(docker ps -a --filter "name=$NOMBRE" --format "{{.Names}}" | wc -l)" -ne 0 ]; then
        # Check if the container is stopped and start it if necessary
        if [ "$(docker inspect -f '{{.State.Running}}' $NOMBRE)" == "false" ]; then
            echo -e "${YELLOW}Starting existing container $NOMBRE...${RESET}"
            docker start $NOMBRE
        else
            echo -e "${YELLOW}Container $NOMBRE is already running.${RESET}"
        fi
    else
        # Create a new container if it doesn't exist
        echo "${YELLOW}Creating a new container $NOMBRE...${RESET}"
        sudo docker run -d -v "$PWD":/root --security-opt seccomp:unconfined -ti --name $NOMBRE agodio/itba-so-multi-platform:3.0
    fi
}

# Function to compile the project
compile() {


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


# Define colors
RED='\033[31m'
YELLOW='\033[33m'
GREEN='\033[32m'
RESET='\033[0m'


# Function to color "error" and "warning" in the output
color_output() {
    awk '{ gsub(/[Ee][Rr][Rr][Oo][Rr]/, "'${RED}'&'${RESET}'"); gsub(/[Ww][Aa][Rr][Nn][Ii][Nn][Gg]/, "'${YELLOW}'&'${RESET}'"); print }'
}


# Main script:


container

if  [ $# -eq 0 ]
then
      
        echo -e "${YELLOW}Compiling...${RESET}"
        compile

else

  case "$1" in
    -w)
        echo -e "${YELLOW}Compiling and only showing warnings...${RESET}"
        WARNINGS=$(compile | egrep -i "warning" | color_output)
        echo -e "$WARNINGS"

        warn_count=$(echo "$WARNINGS" | wc -l)

        if [ $warn_count -eq 0 ]
        then 
          
            echo -e "${GREEN}No warnings found!${RESET}"

        else

            echo -e "${YELLOW}Number of warnings: $warn_count${RESET}"
        fi
        ;;

    -e)
        echo -e "${YELLOW}Compiling and only showing errors...${RESET}"
        ERRORS=$(compile | egrep -i "error" | color_output)
        echo -e "$ERRORS"
        
        err_count=$(echo "$ERRORS" | wc -l)
        
        if [ $err_count -eq 0 ]
        then 
          
            echo -e "${GREEN}No errors found!${RESET}"

        else

            echo -e "${RED}Number of (possible) errors: $err_count${RESET}"
        fi
        ;;

    -a)
        echo -e "${YELLOW}Compiling and only showing errors and warnings...${RESET}"
        OUTPUT=$(compile | egrep -i "error|warning" | color_output)
        echo -e "$OUTPUT"

        err_count=$(echo "$OUTPUT" | egrep -i "error" | wc -l)
        warn_count=$(echo "$OUTPUT" | egrep -i "warning" | wc -l)
        
        if [ $err_count -eq 0 ]
        then 
          
            echo -e "${GREEN}No errors found!${RESET}"

        else

            echo -e "${RED}Number of (possible) errors: $err_count${RESET}"
        fi

        if [ $warn_count -eq 0 ]
        then 
          
            echo -e "${GREEN}No warnings found!${RESET}"

        else

            echo -e "${YELLOW}Number of warnings: $warn_count${RESET}"
        fi
        ;;

    -r)
        compile &> /dev/null || { echo "${RED}Compilation failed!${RESET}"; exit 1; }
        echo -e "${YELLOW}Running project...${RESET}"
        run
        ;;
    -d)
        compile &> /dev/null || { echo "${RED}Compilation failed!${RESET}"; exit 1; }
        echo -e "${YELLOW}Running debug mode...${RESET}"
        run "-d"
        ;;
    *)
        echo -e "${RED}ERROR.${RESET}"
        echo "Usage: $0 [-w for warnings] [-r to run] [-d for debug mode]"
        ;;
  esac
fi
