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
    if [ "$(docker ps -a --filter "name=$NOMBRE" --format "{{.Names}}")" ]; then
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
    docker start $NOMBRE
    docker exec -it $NOMBRE make clean -C/root/Toolchain
    docker exec -it $NOMBRE make clean -C/root/
    docker exec -it $NOMBRE make -C/root/Toolchain
    docker exec -it $NOMBRE make -C/root/
    docker stop $NOMBRE
}

# Define colors
RED='\033[31m'
YELLOW='\033[33m'
GREEN='\033[32m'
RESET='\033[0m'

# Function to show compilation output
show_output() {
    local output_type=$1
    local output=$(compile 2>&1 | egrep -i "$output_type")

    if [[ -z "$output" ]]; then
        echo -e "${GREEN}No $output_type found!${RESET}"
    else
        echo -e "$output"
        echo -e "${YELLOW}Number of $output_type: $(echo "$output" | wc -l)${RESET}"
    fi
}

# Main script:
container

if [ $# -eq 0 ]; then
    echo -e "${YELLOW}Compiling...${RESET}"
    compile
else
    case "$1" in
        -w)
            echo -e "${YELLOW}Compiling and only showing warnings...${RESET}"
            show_output "warning"
            ;;
        -e)
            echo -e "${YELLOW}Compiling and only showing errors...${RESET}"
            show_output "error"
            ;;
        -a)
            echo -e "${YELLOW}Compiling and showing errors and warnings...${RESET}"
            show_output "error"
            show_output "warning"
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
            echo "Usage: $0 [-w for warnings] [-e for errors] [-a for both warnings and errors] [-r to run] [-d for debug mode]"
            ;;
    esac
fi
