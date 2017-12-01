#ifndef TABLES
#define TABLES

#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <cctype>

using namespace std;

struct Page {
	Page *prev = NULL;
	Page *next = NULL;
	int entryID = 0;
	string physicalAddress = "0x00";
};

class PageTable {
public:
	Page *top = NULL;
	Page *end = NULL;
	PageTable *prev = NULL;
	PageTable *next = NULL;
	int entryID = 0;
	string physicalAddress = "0x00";
	PageTable(){};
	PageTable(int segmentLength);
	void insert(int pageID);
};

class SegmentTable {
public:
	PageTable *top = NULL;
	PageTable *end = NULL;
	string physicalAddress = "0x00";
	SegmentTable();
	void insert(int pageTableID);
	void print();
};

//This is the default constructor for the Segment Table
SegmentTable::SegmentTable() {
	insert(1);//One Page Table per Segment Table
}

//This is used to insert a new Page Table entry into the Segment Table
void SegmentTable::insert(int pageTableID) {
	PageTable *newEntry = new PageTable(4);
	newEntry->entryID = pageTableID;
	//If the top of the table is empty then insert at the top
	if (top == NULL) {
		top = newEntry;
	}
	//Else look for the end of the table
	else {
		PageTable *itr;
		itr = top;
		while (itr->next != NULL) {
			itr = itr->next;
		}
		itr->next = newEntry;
		newEntry->prev = itr;
		end = newEntry;
	}
}

void SegmentTable::print() {
	PageTable *pageTableItr;
	pageTableItr = this->top;
	while (pageTableItr != NULL) {
		cout << "Table ID: " << setw(2) << pageTableItr->entryID << " Memory Address: " << setfill('0') << pageTableItr->physicalAddress << setfill(' ') << endl;
		
		Page *pageItr;
		pageItr = pageTableItr->top;
		while (pageItr != NULL) {
			cout << "Page ID: " << setw(3) << pageItr->entryID << " Memory Address: 0x"<< setfill('0') << setw(2) << pageItr->physicalAddress << setfill(' ') << endl;
			pageItr = pageItr->next;
		}
		
		pageTableItr = pageTableItr->next;
	}
}


PageTable::PageTable(int segmentLength) {
	for (int i = 1; i <= segmentLength; i++) {
		insert(i);
	}
}

void PageTable::insert(int pageID) {
	int memAddr = rand() % 64 + 1;
	stringstream ss;
	ss << std::hex << memAddr;
	
	Page *newEntry = new Page;
	newEntry->entryID = pageID;
	newEntry->physicalAddress = ss.str();
	for (int i = 0; i < 2; i++) {
		if (isalpha(newEntry->physicalAddress[i])) {
			newEntry->physicalAddress[i] = toupper(newEntry->physicalAddress[i]);
		}
	}
	//Insert at the top of the table if table is empty
	if (top == NULL) {
		top = newEntry;
	}
	//Else insert at the bottom of the table
	else {
		Page *itr;
		itr = top;
		while (itr->next != NULL) {
			itr = itr->next;
		}
		itr->next = newEntry;
		newEntry->prev = itr;
		end = newEntry;
	}
}

#endif
