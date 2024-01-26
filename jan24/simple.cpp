// Class Notes 1/24/24 & 1/26/24
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

class DataItem { // use sizeof() - a macro to get the size in bytes
    public:
    bool hourly;
    double salary;
    int id;
    DataItem(bool newHourly=true, double newSalary=0.0, int newId=0) {
        hourly = newHourly;
        salary = newSalary;
        id = newId;
    }
    void read(ifstream &in) { // We can use "this" as the location to read in DataItem
        in.read((char *)this, sizeof(DataItem)); // Just like below but read the entire size of the DataItem
        // Have to typecast as a char * (because it is expecting a buffer and we have a DataItem *)
    }
    void read(int which, ifstream &in) {
        in.seekg(which*sizeof(DataItem)); // Just like below but use location declared in parameter
        read(in); // Call the function above - don't rewrite code!
    }
    void write(ofstream &out) {
        out.write((char *)this, sizeof(DataItem));
    }
    void write(int which, ofstream &out) {
        out.seekp(which*sizeof(DataItem)); // Use seekp for output (seekg for input)
        write(out);
    }
    friend ostream & operator << (ostream &out, const DataItem &data) { // Friend makes it not pass "this"
        out << "Hourly: ";
        if (data.hourly) out << "Yes ";
        else out << "No ";
        return out << "Salary: " << data.salary << " " << "Id: " << data.id << endl;
    }
};

int main() {
    DataItem d(true, 10000.0, 1);
    DataItem e(false, 20000.0,2);
    cout << sizeof(bool) << endl;
    cout << sizeof(double) << endl;
    cout << sizeof(int) << endl;
    cout << sizeof(DataItem) << endl;
    cout << d << e; // Use the override function we wrote above

    ofstream out;
    out.open("test.dat"); // Write to "test.dat" file
    d.write(out);
    e.write(out);
    out.close();

    /* 1/24 Notes for writing
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

    */
    // Now we want to read just "Hello again" but jump right there
    // Jump where you want in a file -> essential concept to dbs

    ifstream in;
    in.open("test.dat"); // Open the file that we wrote DataItems to before
    DataItem q;
    q.read(1, in);
    cout << q;
    in.close();
    /*in.open("test.dat");
    in.seekg(100); // Seek direction to start at start, end, or current location, etc.
    //in.seekg(0); // This will print them in reverse: "Hello again Hello Mia" if duplicated section above
    in.read(buffer, 100); // Read 100 bytes of buffer
    cout << buffer << endl;
    in.close();*/
    return 0;
}