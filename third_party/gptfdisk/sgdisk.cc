// sgdisk.cc
// Command-line-based version of gdisk. This program is named after sfdisk,
// and it can serve a similar role (easily scripted, etc.), but it's used
// strictly via command-line arguments, and it doesn't bear much resemblance
// to sfdisk in actual use.
//
// by Rod Smith, project began February 2009; sgdisk begun January 2010.

/* This program is copyright (c) 2009-2011 by Roderick W. Smith. It is distributed
  under the terms of the GNU GPL version 2, as detailed in the COPYING file. */

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>

#include "gptcl.h"

using namespace std;

#define MAX_OPTIONS 50

/*
 * Dump partition details in a machine readable format:
 *
 * DISK[mbr|gpt][guid]
 * PART[n][type][guid]
 */
static int ohos_dump(char* device) {
   BasicMBRData mbrData;
   GPTData gptData;
   GPTPart partData;
   int numParts = 0;

   if (!mbrData.ReadMBRData((string)device)) {
     cerr << "Failed to read MBR" << endl;
     return 8;
   }

   switch(mbrData.GetValidity()) {
     case mbr:
       cout << "DISK mbr" << endl;
       for (int i = 0; i < MAX_MBR_PARTS; i++) {
         if(mbrData.GetLength(i) > 0) {
           cout << "PART " << (i + 1) << " " << hex << (int)mbrData.GetType(i) << dec << endl;
         }
       }
       break;
     case gpt:
       gptData.JustLooking();
       gptData.BeQuiet();
       if(!gptData.LoadPartitions((string)device)) {
         cerr << "Failed to read GPT" << endl;
         return 9;
       }

       cout << "DISK gpt " << gptData.GetDiskGUID() << endl;
       numParts = gptData.GetNumParts();
       for (int i = 0; i < numParts; i++) {
         partData = gptData[i];
         if (partData.GetFirstLBA() > 0) {
           cout << "PART " << (i + 1) << " " << partData.GetType() << " " << partData.GetUniqueGUID() << " "
               << partData.GetDescription() << endl;
         }
       }
       break;
     default:
       cerr << "Unknown partition table" << endl;
       return 10;
   }

   return 0;
}

int main(int argc, char *argv[]) {
   for (int i = 0; i < argc; i++) {
     if (!strcmp("--ohos-dump", argv[i])) {
       return ohos_dump(argv[i + 1]);
     }
   }
   GPTDataCL theGPT;
   return theGPT.DoOptions(argc, argv);
} // main

