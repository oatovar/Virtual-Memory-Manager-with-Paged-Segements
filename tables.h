#ifndef TABLES
#define TABLES

using namespace std;

class SegmentTable {
private:
	PageTable *top;
	PageTable *end;
	int segmentLength;
public:
	SegmentTable();
	SegmentTable(int segmentLength);
	void insert(int pageTableID);
};

//This is the default constructor for the Segment Table
SegmentTable::SegmentTable(segmentLength) {
	for (int i = 0; i < segmentLength; i++) {
		insert(i);
	}
}

//This is used to insert a new Page Table entry into the Segment Table
void SegmentTable::insert(int pageTableID) {
	PageTable *newEntry = new PageTable;
	newEntry->entryID = pageTableID;
	//If the top of the table is empty then insert at the top
	if (top == NULL) {
		top = newEntry;
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
	PageTable *prev;
	PageTable *next;
	int entryID = 0;
	PageTable(){};
	PageTable(int pageSize);
	void insert(i);
};



PageTable::PageTable(int pageSize) {
	for (int i = 0; i < pageSize; i++) {
		insert(i);
	}
}

void PageTable::insert(int pageID) {
	Page *newEntry = new Page;
	newEntry->entryID = pageID;
	//Insert at the top of the table if table is empty
	if (top == NULL) {
		top = newEntry;
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
