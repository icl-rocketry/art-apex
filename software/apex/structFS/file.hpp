#pragma once

template <typename T>
class File {
public:
    // DO NOT initialise outside FS unless you definitely know what you're doing
    File(uint8_t* start, uint8_t* end);

private:
    uint8_t* start;    
    uint8_t* end;   
};