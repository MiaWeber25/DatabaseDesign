// Feb 2nd:
// Read a CSV file into c++ -> "How many businesses are in good standing in Mesa County?"

// We can use seekg() to determine which delim to use (, or "")

// Feb 5th:
// Only read the entire file once to put together an index so that next time we can just lookup by ID quickly
// Use a map class to create an index (based on a key) for quick lookup of "entityid"
// Add indexes to db (idea of lookup tables) to improve performance and increase speed of queries.

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <cstdlib>

using namespace std;

vector<string> readRow(ifstream &in) { // Read a row from the current position in the file
    vector<string> row; 
    string strrow;
    getline(in, strrow); // optional third param for delim
    stringstream ss(strrow); // make a string stream
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
    return row;
}

struct entityIdIndex { // write struct of index to disk
    long entityId;
    long pos;
};

/* What if I had an index by entityName?
struct entityNameIndex {
    char[30] entityName; // remember writing strings to disk is bad
    long pos;
};
*/

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
    int id = headingIndex["entityid"];
    int city = headingIndex["principalcity"];
    int status = headingIndex["entitystatus"];
    map<long,long> lookup; // lookup table -> avoid going through the whole file every time (id,position)
    ifstream findex; // THIS METHOD IS VERY QUICK COMPARED TO BEFORE
    findex.open("entityId.idx");
    while (findex.good()) {
        entityIdIndex idIndex;
        findex.read((char *)&idIndex, sizeof(entityIdIndex));
        lookup[idIndex.entityId] = idIndex.pos; // add it to the lookup table
    }
    findex.close();

    /*ofstream findex;
    findex.open("entityId.idx"); // need a place to write the index to
    while (in.good()) {
        long pos = in.tellg(); // Where am I in the file?
        vector<string> row;
        row=readRow(in); // Go use the function to read a row from the current position in the file
        entityIdIndex idIndex; // use the struct created above
        idIndex.pos=pos;
        idIndex.entityId=atol(row[id].c_str());
        findex.write((char *)&idIndex, sizeof(idIndex)); // write the index to the file
        //cout << pos << " , " << row[id] << endl; // output the location in the file and the id of the entity there (don't have to read the whole file)
        //lookup[atol(row[id].c_str())] = pos; // update the lookup table with the relevent info (convert string to long and streamstring to c string)
        if (city < row.size() && status < row.size() && row[city] == "Grand Junction" && row[status] == "Good Standing") {
            counter++;
        }
    } 
    findex.close();*/
    //cout << "There are " << counter << " businesses in Grand Junction in good standing." << endl;
    in.close();
    in.open("Business_Entities_in_Colorado_20240202.csv");
    // long ids[]={20231166809,19281187282};
    long first = 18861217679;
    long last = 20238306968;
    for (int i=0;i<10000;i++) { // Do 10000 random lookups -> much faster this time (BECAUSE YOU HAVE INDEX'!!!!! Without them it would take days)
        long idx = rand()%(last-first)+first; // generate an integer in the range of the first and last entityId
        auto entry = lookup.find(idx);
        if (entry != lookup.end()) {
            //in.seekg(lookup[idx]); // Use the map to find the location of the entity id from the array of ids
            in.seekg(entry->second); // Do this instead of line above (get the second part of the pair - the pos). Save the extra lookup
            vector<string> check = readRow(in);
            for(auto column: check) {
                cout << column << " , ";
            }
            cout << endl;
        } //else {
            //cout << "EntityId does not exist: " << idx << endl;
        //} 
    }
    in.close();
    return 0;
}

// 13722 is the answer (I got 13705)