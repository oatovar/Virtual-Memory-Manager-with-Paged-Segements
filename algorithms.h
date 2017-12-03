#ifndef ALGORITHMS
#define ALGORITHMS

#include <iostream>
#include <iomanip>
#include <cmath>
#include <list>

using namespace std;

void reset(list<frame> currentFrames, processObject processProperties[], int k)
{
	list<frame>::iterator frameItr = currentFrames.begin();
	//Reset the frames table to simulate a fresh start
	while(frameItr != currentFrames.end()) {
		frameItr->addressSpace = 0;
		frameItr->segment = 0;
		frameItr->page = 0;
		frameItr++;
	}
	//Reset process objects to reinitialize frame count and page faults to zero
	for (int i = 0; i < k; i++) {
		processProperties[i].pageFault = 0;
		processProperties[i].currentFrameCount = 0;
	}
	return;
}

void printFramesTable(list<frame> currentFrames)
{
	cout << endl << "Frames Table" << endl;
	for (list<frame>::iterator itr = currentFrames.begin(); itr != currentFrames.end(); ++itr) {
		cout << "Address Space: " << setw(4) << itr->addressSpace << " "
			 << "Segment #: " << setw(3) << itr->segment << " "
			 << "Page #: " << setw(3) << itr->page
			 << endl;
	}
	cout << endl;
}

void fifo(list<frame> currentFrames, list<memoryRequest> currentRequests, processObject processProperties[], int framesPerProcess, int k)
{
	reset(currentFrames, processProperties, k);
	cout << endl << "Starting FIFO replacement algorithm." << endl;
	list<memoryRequest>::iterator requestItr = currentRequests.begin();
	list<frame>::iterator frameItr = currentFrames.begin();
	int totalPageFaults = 0;
	bool found = false;
	//int addressSpace = requestItr->addressSpace - 100;
	//printFramesTable(currentFrames);
	while (requestItr != currentRequests.end()) {
		found = false;
		if (requestItr->offset != -1) {
			if (processProperties[requestItr->addressSpace - 100].currentFrameCount < framesPerProcess) {
				while(frameItr != currentFrames.end()) {
                    if ((frameItr->addressSpace == requestItr->addressSpace) && (frameItr->segment == requestItr->segment) && (frameItr->page == requestItr->page)) {
                        //Frame already contains page needed. No need to use a new frame buffer.
                        //cout << "Page in frame buffer." << endl;
                        found = true;
                        frameItr = currentFrames.begin();
                        requestItr++;
                        break;
                    }
                    else {
                        //If frame occupied by another page either by the same process or another process
                        frameItr++; //Advance to next frame. In this case moves on to an older frame.
                    }
				}
				frameItr = currentFrames.begin();
				while((frameItr != currentFrames.end()) && (found == false)) {
					if (frameItr->addressSpace == 0) {
                        //Copy page to the frame buffer
                        //cout << "Copying page into frame buffer." << endl;
                        frameItr->addressSpace = requestItr->addressSpace;
                        frameItr->segment = requestItr->segment;
                        frameItr->page = requestItr->page;
                        //Increase the count of frames that currently belong to the process
                        processProperties[requestItr->addressSpace - 100].currentFrameCount++;
                        processProperties[requestItr->addressSpace - 100].pageFault++;
                        //Break out of loop and move on to the next request
                        found = true;
                        frameItr = currentFrames.begin();
                        requestItr++;
                        break;
                    }
                    else {
                    	frameItr++;
                    }
            	}
			}
            else {
            	frameItr = currentFrames.begin();
            	while(frameItr != currentFrames.end()) {
					if ((frameItr->addressSpace == requestItr->addressSpace) && (frameItr->segment == requestItr->segment) && (frameItr->page == requestItr->page)) {
						//Frame already contains page needed. No need to use a new frame buffer.
						//cout << "Page in frame buffer." << endl;
						found = true;
						frameItr = currentFrames.begin();
						requestItr++;
						break;
					}
					else {
						frameItr++;
					}
				}
				frameItr = currentFrames.begin();
				while(frameItr != currentFrames.end() && (found == false)) {
					if (frameItr->addressSpace == requestItr->addressSpace) {
						//cout << "Replacing frame buffer." << endl;
			            //Remove the first page that came in
			            frameItr = currentFrames.erase(frameItr);
			            //Update frames table to reflect the new order of pages in frame buffers
			            frame newFrame;
			            newFrame.addressSpace = requestItr->addressSpace;
			            newFrame.segment = requestItr->segment;
			            newFrame.page = requestItr->page;
			            currentFrames.push_back(newFrame);
			            processProperties[requestItr->addressSpace - 100].pageFault++;
			            //Break out of loop and move on to the next request
			            found = true;
			            frameItr = currentFrames.begin();
			            requestItr++;
             		    break;
            	    }
            	    else {
            	        frameItr++;//Go to the next frame. Since it's LIFO we move to an older frame.
            	    }
                }
            }
		}
        //If the request does not belong to the process move to the next process
        else {
        	//cout << "Process " << requestItr->addressSpace << " finished!" << endl;
        	frameItr = currentFrames.begin();
			requestItr++;
		}
		//printFramesTable(currentFrames);
	}
	cout << endl << "RESULTS" << endl;
	cout << "---------------------------------------------" << endl;
	for (int i = 0; i < k; i++) {
		cout << "Page Faults for Process " << i + 100 << ": "
			 << processProperties[i].pageFault << endl;
		totalPageFaults += processProperties[i].pageFault;
	}
	cout << "Total Page Faults: " << totalPageFaults << endl;
	cout << "---------------------------------------------" << endl;
}

void lru()
{

}

void lifo(list<frame> currentFrames, list<memoryRequest> currentRequests, processObject processProperties[], int framesPerProcess, int k)
{
	reset(currentFrames, processProperties, k);
	cout << endl << "Starting LIFO replacement algorithm." << endl;
    list<memoryRequest>::iterator requestItr = currentRequests.begin();
    list<frame>::iterator frameItr = currentFrames.begin();
    int totalPageFaults = 0;
    bool found = false;
    //int addressSpace = requestItr->addressSpace - 100;
    while (requestItr != currentRequests.end()) {
    	found = false;
        if (requestItr->offset != -1) {
            if (processProperties[requestItr->addressSpace - 100].currentFrameCount < framesPerProcess) {
				while(frameItr != currentFrames.end()) {
                    if ((frameItr->addressSpace == requestItr->addressSpace) && (frameItr->segment == requestItr->segment) && (frameItr->page == requestItr->page)) {
                        //Frame already contains page needed. No need to use a new frame buffer.
                        //cout << "Page in frame buffer." << endl;
                        found = true;
                        frameItr = currentFrames.begin();
                        requestItr++;
                        break;
                    }
                    else {
                        //If frame occupied by another page either by the same process or another process
                        frameItr++; //Advance to next frame. In this case moves on to an older frame.
                    }
				}
				frameItr = currentFrames.begin();
				while((frameItr != currentFrames.end()) && (found == false)) {
					if (frameItr->addressSpace == 0) {
                        //Copy page to the frame buffer
                        //cout << "Copying page into frame buffer." << endl;
                        frameItr->addressSpace = requestItr->addressSpace;
                        frameItr->segment = requestItr->segment;
                        frameItr->page = requestItr->page;
                        //Increase the count of frames that currently belong to the process
                        processProperties[requestItr->addressSpace - 100].currentFrameCount++;
                        processProperties[requestItr->addressSpace - 100].pageFault++;
                        //Break out of loop and move on to the next request
                        found = true;
                        frameItr = currentFrames.begin();
                        requestItr++;
                        break;
                    }
                    else {
                    	frameItr++;
                    }
            	}
            }
            else {
            	frameItr = currentFrames.begin();
            	while(frameItr != currentFrames.end()) {
					if ((frameItr->addressSpace == requestItr->addressSpace) && (frameItr->segment == requestItr->segment) && (frameItr->page == requestItr->page)) {
						//Frame already contains page needed. No need to use a new frame buffer.
						//cout << "Page in frame buffer." << endl;
						found = true;
						frameItr = currentFrames.begin();
						requestItr++;
						break;
					}
					else {
						frameItr++;
					}
				}
				frameItr = currentFrames.begin();
				while((frameItr != currentFrames.end()) && (found == false)) {
					if (frameItr->addressSpace == requestItr->addressSpace) {
						//cout << "Replacing frame buffer." << endl;
			            //Remove the first page that came in
			            frameItr = currentFrames.erase(frameItr);
			            //Update frames table to reflect the new order of pages in frame buffers
			            frame newFrame;
			            newFrame.addressSpace = requestItr->addressSpace;
			            newFrame.segment = requestItr->segment;
			            newFrame.page = requestItr->page;
			            currentFrames.push_front(newFrame);
			            processProperties[requestItr->addressSpace - 100].pageFault++;
			            //Break out of loop and move on to the next request
			            found = true;
			            frameItr = currentFrames.begin();
			            requestItr++;
             		    break;
            	    }
            	    else {
            	        frameItr++;//Go to the next frame. Since it's LIFO we move to an older frame.
            	    }
                }
            }
        }
        //If the request does not belong to the process move to the next process
        else {
        	//cout << "Process " << requestItr->addressSpace << " finished!" << endl;
        	frameItr = currentFrames.begin();
            requestItr++;
        }
    }
	cout << endl << "RESULTS" << endl;
	cout << "---------------------------------------------" << endl;
	for (int i = 0; i < k; i++) {
		cout << "Page Faults for Process " << i + 100 << ": "
			 << processProperties[i].pageFault << endl;
		totalPageFaults += processProperties[i].pageFault;
	}
	cout << "Total Page Faults: " << totalPageFaults << endl;
	cout << "---------------------------------------------" << endl;
}

void ldf(list<frame> currentFrames, list<memoryRequest> currentRequests, processObject processProperties[], int framesPerProcess, int k)
{
	reset(currentFrames, processProperties, k);
	cout << endl << "Starting LDF replacement algorithm." << endl;
	list<frame>::iterator frameItr = currentFrames.begin();
	list<frame>::iterator replacementFrame;
	list<memoryRequest>::iterator requestItr = currentRequests.begin();
	int totalPageFaults = 0;
	bool found = false;
	while(requestItr != currentRequests.end()) {
		found = false;
		if (requestItr->offset != -1) {
		        if (processProperties[requestItr->addressSpace - 100].currentFrameCount < framesPerProcess) {
					while(frameItr != currentFrames.end()) {
		                if ((frameItr->addressSpace == requestItr->addressSpace) && (frameItr->segment == requestItr->segment) && (frameItr->page == requestItr->page)) {
		                    //Frame already contains page needed. No need to use a new frame buffer.
		                    //cout << "Page in frame buffer." << endl;
		                 	found = true;
		                    frameItr = currentFrames.begin();
		                    requestItr++;
		                    break;
		                }
		                else {
		                    //If frame occupied by another page either by the same process or another process
		                    frameItr++; //Advance to next frame. In this case moves on to an older frame.
		                }
					}
					frameItr = currentFrames.begin();
					while((frameItr != currentFrames.end()) && (found == false)) {
						if (frameItr->addressSpace == 0) {
		                    //Copy page to the frame buffer
		                    //cout << "Copying page into frame buffer." << endl;
		                    frameItr->addressSpace = requestItr->addressSpace;
		                    frameItr->segment = requestItr->segment;
		                    frameItr->page = requestItr->page;
		                    //Increase the count of frames that currently belong to the process
		                    processProperties[requestItr->addressSpace - 100].currentFrameCount++;
		                    processProperties[requestItr->addressSpace - 100].pageFault++;
		                    //Break out of loop and move on to the next request
		                    found = true;
		                    frameItr = currentFrames.begin();
		                    requestItr++;
		                    break;
		                }
		                else {
		                	frameItr++;
		                }
		        	}
		        }
		        else {
		        	frameItr = currentFrames.begin();
		        	while(frameItr != currentFrames.end()) {
						if ((frameItr->addressSpace == requestItr->addressSpace) && (frameItr->segment == requestItr->segment) && (frameItr->page == requestItr->page)) {
							//Frame already contains page needed. No need to use a new frame buffer.
							//cout << "Page in frame buffer." << endl;
							found = true;
							frameItr = currentFrames.begin();
							requestItr++;
							break;
						}
						else {
							frameItr++;
						}
					}
					/*
					 * If frame was not in memory then will start to search for the longest distance frame
					 * You first start the algorithm by finding the first frame that belongs to the pid/address space
					 * and set that as the frame to replace. After, you continue to search for a frame that belong to that
					 * address and is farther away. If you find such a frame you set the replacement frame iterator to that frame.
					 * Once you have searched all the frames you delete the frame insert the new one needed.
					*/
					frameItr = currentFrames.begin();
					while(frameItr != currentFrames.end()) {
						if (frameItr->addressSpace == requestItr->addressSpace) {
							replacementFrame = frameItr;
							frameItr++;
							break;
						}
						else {
							frameItr++;
						}
					}
					while((frameItr != currentFrames.end()) && (found == false)) {
						if (frameItr->addressSpace == requestItr->addressSpace) {
							if (abs(requestItr->segment - frameItr->segment) > abs(requestItr->segment - replacementFrame->segment)) {
								//if frameItr segment difference is higher than replacementFrameItr segment difference update the
								//the replacment frame iterator
								replacementFrame = frameItr;
							}
							else if (abs(requestItr->segment - frameItr->segment) == abs(requestItr->segment - replacementFrame->segment)) {
								//If there is no difference you must check to see which frame results in the largest page difference
								int requestAddress = 0;
								int currentFrameAddress = 0;
								int replacementFrameAddress = 0;
								int segmentSizeConstant = 16; //Constant used to calculate page difference
								/*
								 * Here you must calculate the raw logical address for the request,
								 * current frame to replace, and the frame that is currently being analyzed
								*/
								currentFrameAddress = frameItr->segment * segmentSizeConstant + frameItr->page;
								replacementFrameAddress = replacementFrame->segment * segmentSizeConstant + replacementFrame->page;
								requestAddress = requestItr->segment * segmentSizeConstant + requestItr->page;
								if (abs(requestAddress - currentFrameAddress) > abs(requestAddress - replacementFrameAddress)) {
									replacementFrame = frameItr;
								}
							}
							else {
								//Just need to advance to the next frame and repeat process
							}
						}
						frameItr++;
					}
					//cout << "Replacing frame buffer." << endl;
			        //Remove the first page that came in
			        frameItr = currentFrames.erase(replacementFrame);
			        //Update frames table to reflect the new order of pages in frame buffers
			        frame newFrame;
			        newFrame.addressSpace = requestItr->addressSpace;
			        newFrame.segment = requestItr->segment;
			        newFrame.page = requestItr->page;
			        currentFrames.push_front(newFrame);
			        processProperties[requestItr->addressSpace - 100].pageFault++;
			        //Break out of loop and move on to the next request
			        found = true;
			        frameItr = currentFrames.begin();
			        requestItr++;
				}
		}
		else {
			frameItr = currentFrames.begin();
			requestItr++;
		}            
	}
	cout << endl << "RESULTS" << endl;
	cout << "---------------------------------------------" << endl;
	for (int i = 0; i < k; i++) {
		cout << "Page Faults for Process " << i + 100 << ": "
			 << processProperties[i].pageFault << endl;
			 
		totalPageFaults += processProperties[i].pageFault;
	}
	cout << "Total Page Faults: " << totalPageFaults << endl;
	cout << "---------------------------------------------" << endl;
}

void opt()
{
}

void workingSet()
{
}

#endif
