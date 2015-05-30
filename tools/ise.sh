
XILINX_SETTINGS="${XILINX_SETTINGS:-/opt/Xilinx/14.7/ISE_DS/settings64.sh}"


source "$XILINX_SETTINGS"
echo "Starting ISE in the background..."
ise & 
