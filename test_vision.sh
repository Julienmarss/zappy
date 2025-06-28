#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

SERVER_PORT=4242
SERVER_PID=""
TEST_COUNT=0
PASS_COUNT=0

# Function to start server
start_server() {
    echo -e "${BLUE}🚀 Starting Zappy server...${NC}"
    ./zappy_server -p $SERVER_PORT -x 10 -y 10 -n team1 -c 5 -f 1000 > /dev/null 2>&1 &
    SERVER_PID=$!
    sleep 2
    
    if kill -0 $SERVER_PID 2>/dev/null; then
        echo -e "${GREEN}✅ Server started (PID: $SERVER_PID)${NC}"
        return 0
    else
        echo -e "${RED}❌ Failed to start server${NC}"
        return 1
    fi
}

# Function to stop server
stop_server() {
    if [[ -n "$SERVER_PID" ]]; then
        echo -e "${BLUE}🛑 Stopping server...${NC}"
        kill $SERVER_PID 2>/dev/null
        wait $SERVER_PID 2>/dev/null
        echo -e "${GREEN}✅ Server stopped${NC}"
    fi
}

# Function to send command and get response
send_command() {
    local command="$1"
    local timeout="${2:-3}"
    
    echo "$command" | timeout $timeout nc localhost $SERVER_PORT 2>/dev/null
}

# Function to test vision for a specific level
test_vision_level() {
    local level="$1"
    local expected_tiles="$2"
    
    echo -e "${YELLOW}📋 Testing vision level $level...${NC}"
    
    # Connect and get to level
    local response=$(
        (
            echo "team1"
            sleep 0.5
            
            # Level up if needed
            for ((i=1; i<level; i++)); do
                echo "Incantation"
                sleep 0.5
            done
            
            echo "Look"
            sleep 1
        ) | timeout 10 nc localhost $SERVER_PORT 2>/dev/null
    )
    
    # Count tiles in response
    local tile_count=$(echo "$response" | grep -o ',' | wc -l)
    tile_count=$((tile_count + 1)) # Add 1 for the last tile
    
    TEST_COUNT=$((TEST_COUNT + 1))
    
    if [[ $tile_count -eq $expected_tiles ]]; then
        echo -e "${GREEN}✅ Level $level: Expected $expected_tiles tiles, got $tile_count${NC}"
        PASS_COUNT=$((PASS_COUNT + 1))
        return 0
    else
        echo -e "${RED}❌ Level $level: Expected $expected_tiles tiles, got $tile_count${NC}"
        echo -e "${RED}   Response: $response${NC}"
        return 1
    fi
}

# Function to test orientation
test_orientation() {
    echo -e "${YELLOW}📋 Testing orientation changes...${NC}"
    
    local response=$(
        (
            echo "team1"
            sleep 0.5
            echo "Look"
            sleep 0.5
            echo "Right"
            sleep 0.5
            echo "Look"
            sleep 0.5
            echo "Left"
            sleep 0.5
            echo "Left"
            sleep 0.5
            echo "Look"
            sleep 1
        ) | timeout 10 nc localhost $SERVER_PORT 2>/dev/null
    )
    
    local look_count=$(echo "$response" | grep -c '\[.*\]')
    
    TEST_COUNT=$((TEST_COUNT + 1))
    
    if [[ $look_count -eq 3 ]]; then
        echo -e "${GREEN}✅ Orientation test: Got $look_count vision responses${NC}"
        PASS_COUNT=$((PASS_COUNT + 1))
        return 0
    else
        echo -e "${RED}❌ Orientation test: Expected 3 responses, got $look_count${NC}"
        return 1
    fi
}

# Function to test basic vision format
test_vision_format() {
    echo -e "${YELLOW}📋 Testing vision format...${NC}"
    
    local response=$(
        (
            echo "team1"
            sleep 0.5
            echo "Look"
            sleep 1
        ) | timeout 5 nc localhost $SERVER_PORT 2>/dev/null
    )
    
    TEST_COUNT=$((TEST_COUNT + 1))
    
    # Check if response contains proper format [...]
    if echo "$response" | grep -q '\[.*player.*\]'; then
        echo -e "${GREEN}✅ Format test: Valid vision format detected${NC}"
        echo -e "${BLUE}   Sample: $(echo "$response" | grep '\[.*\]' | head -1)${NC}"
        PASS_COUNT=$((PASS_COUNT + 1))
        return 0
    else
        echo -e "${RED}❌ Format test: Invalid vision format${NC}"
        echo -e "${RED}   Response: $response${NC}"
        return 1
    fi
}

# Function to test movement and vision
test_movement_vision() {
    echo -e "${YELLOW}📋 Testing movement affects vision...${NC}"
    
    local response=$(
        (
            echo "team1"
            sleep 0.5
            echo "Look"
            sleep 0.5
            echo "Forward"
            sleep 0.5
            echo "Look"
            sleep 1
        ) | timeout 8 nc localhost $SERVER_PORT 2>/dev/null
    )
    
    local look_count=$(echo "$response" | grep -c '\[.*\]')
    
    TEST_COUNT=$((TEST_COUNT + 1))
    
    if [[ $look_count -eq 2 ]]; then
        echo -e "${GREEN}✅ Movement test: Got vision before and after movement${NC}"
        PASS_COUNT=$((PASS_COUNT + 1))
        return 0
    else
        echo -e "${RED}❌ Movement test: Expected 2 vision responses, got $look_count${NC}"
        return 1
    fi
}

# Main test function
run_tests() {
    echo -e "${BLUE}🧪 VISION SYSTEM TESTS${NC}"
    echo -e "${BLUE}=====================${NC}"
    
    # Test basic vision format
    test_vision_format
    echo
    
    # Test orientation
    test_orientation
    echo
    
    # Test movement
    test_movement_vision
    echo
    
    # Test level 1 vision (should see 9 tiles minimum)
    test_vision_level 1 4
    echo
    
    # Summary
    echo -e "${BLUE}📊 TEST SUMMARY${NC}"
    echo -e "${BLUE}===============${NC}"
    echo -e "Total tests: $TEST_COUNT"
    echo -e "${GREEN}Passed: $PASS_COUNT${NC}"
    echo -e "${RED}Failed: $((TEST_COUNT - PASS_COUNT))${NC}"
    
    if [[ $PASS_COUNT -eq $TEST_COUNT ]]; then
        echo -e "${GREEN}🎉 All tests passed!${NC}"
        return 0
    else
        echo -e "${RED}❌ Some tests failed${NC}"
        return 1
    fi
}

# Cleanup function
cleanup() {
    echo -e "\n${BLUE}🧹 Cleaning up...${NC}"
    stop_server
}

# Set trap for cleanup
trap cleanup EXIT INT TERM

# Main execution
echo -e "${BLUE}🔧 ZAPPY VISION SYSTEM TEST${NC}"
echo -e "${BLUE}============================${NC}"

# Check if server exists
if [[ ! -f "./zappy_server" ]]; then
    echo -e "${RED}❌ Error: zappy_server not found. Please compile first.${NC}"
    exit 1
fi

# Check if port is available
if lsof -Pi :$SERVER_PORT -sTCP:LISTEN -t >/dev/null; then
    echo -e "${RED}❌ Error: Port $SERVER_PORT is already in use${NC}"
    exit 1
fi

# Start server and run tests
if start_server; then
    echo
    run_tests
    exit_code=$?
    echo
    exit $exit_code
else
    echo -e "${RED}❌ Cannot start server, aborting tests${NC}"
    exit 1
fi
