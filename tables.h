#ifndef TABLES
#define TABLES

#include <cstdlib>
#include <stringstream>

using namespace std;

class SegmentTable {
private:
	PageTable *top;
	PageTable *end;
public:
	SegmentTable();
	void insert(int pageTableID);
};

//This is the default constructor for the Segment Table
SegmentTable::SegmentTable() {
	insert(1);//One Page Table per Segment Table
}

//This is used to insert a new Page Table entry into the Segment Table
void SegmentTable::insert(int pageTableID) {
	PageTable *newEntry = new PageTable;
	newEntry->entryID = pageTableID;
	//If the top of the table is empty then insert at the top
	if (top == NULL) {
		top = newEntry;
		end = top;
	}
	//Else look for the end of the table
	else {
		PageTable *iterator = new PageTable;
		iterator = top;
		while (iterator->next != NULL) {
			iterator = iterator->next;
		}
		iterator->next = newEntry;
		newEntry->prev = iterator;
		end = newEntry;
	}
}
		

class PageTable {
private:
	Page *top;
	Page *end;
public:
	int entryID = 0;
	PageTable(){};
	PageTable(int segmentLength);
	void insert(i);
};



PageTable::PageTable(int segmentLength) {
	for (int i = 1; i <= segmentLength; i++) {
		insert(i);
	}
}

void PageTable::insert(int pageID) {
	srand(time(NULL));
	int memAddr = rand() % 1000 + 1;
	stringstream sstream;
	sstream << std::hex << memAddr;
	
	Page *newEntry = new Page;
	newEntry->entryID = pageID;
	newEntry->physicalAddress = sstream.str();
	//Insert at the top of the table if table is empty
	if (top == NULL) {
		top = newEntry;
		end = top;
	}
	//Else insert at the bottom of the table
	else {
		Page *iterator = new Page;
		iterator = top;
		while (iterator->next != NULL) {
			iterator = iterator->next;
		}
		iterator->next = newEntry;
		newEntry->prev = iterator;
		end = newEntry;
	}
}

struct Page {
	Page *prev;
	Page *next;
	int entryID = 0;
	string physicalAddress = "0x0";
};

#endif
