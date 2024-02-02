// Feb 2nd:
// Read a CSV file into c++ -> "How many businesses are in good standing in Mesa County?"

// We can use seekg() to determine which delim to use (, or "")

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>

using namespace std;

int main() {
    ifstream in;
    in.open("Business_Entities_in_Colorado_20240202.csv");

    // Headings
    string strheadings;
    getline(in, strheadings); // optional third param for delim
    stringstream ss(strheadings); // make a string stream

    vector<string> headings;
    map<string, int> headingIndex; // so that we can find easier
    int index = 0;
    while (ss.good()) { // good means it doesn't have a failbit set, eofbit, & badbit
        string heading; // seperate into an individual heading
        getline(ss, heading, ',');
        headings.push_back(heading); // add each heading to the vector of all headings
        headingIndex[heading] = index;
        index++;
    }

    // Rows
    int counter = 0;
    int city = headingIndex["principalcity"];
    int status = headingIndex["entitystatus"];
    while (in.good()) {
        string strrow;
        getline(in, strrow); // optional third param for delim
        stringstream ss(strrow); // make a string stream
        vector<string> row;
        while (ss.good()) { // good means it doesn't have a failbit set, eofbit, & badbit
            string value; // seperate into an individual heading
            if (ss.peek() == '"') { // first quote
                ss.get();
                getline(ss, value, '"'); // second quote
                ss.get(); // Assume it is the comma (both quotes are gone, get rid of comma)
            } else {
                getline(ss, value, ','); // it's not a quote so read until comma like normal
            }
            row.push_back(value); // add each heading to the vector of all headings
        }
        if (city < row.size() && status < row.size() && row[city] == "Grand Junction" && row[status] == "Good Standing") {
            counter++;
        }
    } 
    cout << "There are " << counter << " businesses in Grand Junction in good standing." << endl;
    in.close();
    return 0;
}

// 13722 is the answer