# Define colors
RED='\033[31m'
YELLOW='\033[33m'
GREEN='\033[32m'
RESET='\033[0m'

usage() {
  echo -e "${RED}ERROR.${RESET} Usage: ./solve.sh <file>"
  exit
}

echo ""
echo -e "${YELLOW}-------Windows weird characters solver-------${RESET}"
echo "" 


if [ $# -ne 1 ]; then
  usage
fi

if [ -f "$1" ]; then
  echo -e "Solving ${YELLOW}$1${RESET} problems..."

  # Count characters before deletion
  count_r=$(grep -o $'\r' "$1" | wc -l)
  count_nbsp=$(grep -o $'\xA0' "$1" | wc -l)
  count_bspace=$(grep -o $'\b' "$1" | wc -l)
  count_null=$(grep -o $'\x00' "$1" | wc -l)
  count_vtab=$(grep -o $'\v' "$1" | wc -l)
  count_ff=$(grep -o $'\f' "$1" | wc -l)

  # Remove \r but preserve \n (convert \r\n to \n)
  sed -i -e 's/\r$//' "$1"

  # Remove unwanted characters
  sed -i -e 's/\xA0/ /g' "$1"     # Replace non-breaking spaces with regular spaces
  sed -i -e 's/\b//g' "$1"        # Remove backspace characters
  sed -i -e 's/\x00//g' "$1"      # Remove null bytes
  sed -i -e 's/\v//g' "$1"        # Remove vertical tabs
  sed -i -e 's/\f//g' "$1"        # Remove form feed characters

  # Create a list of characters and their counts
  characters=("\\\\r" "non-breaking space" "backspace" "null byte" "vertical tab" "form feed")
  counts=($count_r $count_nbsp $count_bspace $count_null $count_vtab $count_ff)

  removed_any=false

  # Loop through the list and print only if something was removed
  for i in ${!characters[@]}; do
    if [ "${counts[$i]}" -gt 0 ]; then
      echo -e "${GREEN}DONE!${RESET} Removed ${YELLOW}${counts[$i]}${RESET} '${characters[$i]}' characters."
      removed_any=true
    fi
  done

  # If no characters were removed, print a different message
  if [ "$removed_any" = false ]; then
    echo -e "${GREEN}No special characters were found and removed.${RESET}"
  fi

else
  echo -e "${YELLOW}$1${RESET} is not a file"
  usage
fi

