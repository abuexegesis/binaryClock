#include <Utilities.h>
word buildDisplayOut(byte displayMask, int displaySegment) {
    word displayOut;
    displayOut = CATHODES_MASK_GENERAL*256 + ANODES_MASK_GENERAL;
    
    return displayOut;
}  

String testlib(){
    String message = "Test out the library";
    return message;
}

byte digitToBCD(int number) {
    byte bcdEncodedNumber;
    if ((number > 9) || (number < 0)) number = 0;  // one digit safety

    switch (number) {
    case 9:
        bcdEncodedNumber = B00001001;
        break;
    case 8:
        bcdEncodedNumber = B00001000;
        break;
    case 7:
        bcdEncodedNumber = B0000111;
        break;
    case 6:
        bcdEncodedNumber = B00000110;
        break;
    case 5:
        bcdEncodedNumber = B00000101;
        break;
    case 4:
        bcdEncodedNumber = B00000100;
        break;
    case 3:
        bcdEncodedNumber = B00000011;
        break;
    case 2:
        bcdEncodedNumber = B00000010;
        break;
    case 1:
        bcdEncodedNumber = B00000001;
        break;
    case 0:
        bcdEncodedNumber = B00000000;    
    }
    return bcdEncodedNumber;
}

byte twoDigitsToBCD(int twodigits){
    byte bcdEncodedNumber;
    int tens;
    int ones;
    if ((twodigits > 99) || (twodigits < 0)) twodigits = 0;  // two digit safety

    ones = twodigits % 10;
    tens = (twodigits - ones) / 10;

    bcdEncodedNumber = 16*tens + ones;
    return bcdEncodedNumber;

}

shiftAndCarry adjustAnodesByte(byte anodes){
    shiftAndCarry result;
    result.shifted = (byte)(anodes << 2);
    result.carry = B10000000 && anodes;
    if (result.carry == B10000000) {
        result.carry = B00000001;
    }
    else {
        result.carry = B00000000; 
    }
    return result;
}