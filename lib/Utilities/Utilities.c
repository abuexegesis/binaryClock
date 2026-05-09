#include <Utilities.h>
#include <string.h>
word buildDisplayOut(byte displayMask, int displaySegment) {
    word displayOut;
    displayOut = CATHODES_MASK_GENERAL*256 + ANODES_MASK_GENERAL;
    
    return displayOut;
}  

String testlib(){
    String message = "Test out the library";
    return message;
}
