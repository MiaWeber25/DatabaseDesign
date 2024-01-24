// Class Notes 1/24/24
// seekg and tellg -> more interesting movements of reading data off a system
// TTF files (have to read into data structure in chunks and then skip to relevent part to make any sense of the entire file)
// Bitmap image file (BMP) has a file offset to PixelArray in the header 
// Jump to any part of files - you don't have to read the first stuff to read the last stuff.
// TODO: Get ghex running! 

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;

int main() {
    ofstream out;
    out.open("test.dat");
    stringstream ss; // Make a stringstream
    char buffer[100];

    ss << "Hello Mia" << endl;
    //strncpy(buffer, ss.str().c_str(), 100); // Copy the stringstream into the buffer (securely with n=100)
    //out.write(buffer, 100); // What was the memory at the time we ran the program
    
    out.write(ss.str().c_str(), 100);

    ss.clear(); // Clear the stringstream 
    ss << "Hello again" << endl; 
    out.write(ss.str().c_str(), 100); // Write a new message to the buffer
    
    out.close();

    // Now we want to read just "Hello again" but jump right there
    // Jump where you want in a file -> essential concept to dbs

    ifstream in;
    in.open("test.dat");
    in.seekg(100); // Seek direction to start at start, end, or current location, etc.
    //in.seekg(0); // This will print them in reverse: "Hello again Hello Mia" if duplicated section above
    in.read(buffer, 100); // Read 100 bytes of buffer
    cout << buffer << endl;
    return 0;
}