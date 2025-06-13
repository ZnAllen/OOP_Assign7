#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include "AnsiPrint.h"


// a few constant ansi formatting string
const char *init="\x1b[";
const char *endc="m";
const char *hilit="1;";
const char *blink="5;";
const char *recover="\x1b[0m";
const char *fgBase="38;5;";
const char *bgBase="48;5;";
const int kFormatStrSize=30;

/** 
 * This function takes a string and ansi formatting option such as 
 * foreground and background colors, hilighting for foreground color,
 * and blinking option and then send the appropriate string to the
 * standard output. 
 * The options except for the foreground color are optional. 
 */
std::string
AnsiPrint(const char *str, Color fg, Color bg, bool hi, bool blinking) {

    // kick out exceptional case
    if ((str==NULL)||(strlen(str)==0))
        return "";
    // creating foreground and background options
    char *foreground=strdup(fgBase);
    //foreground[5]+=fg;
	int numF = static_cast<int> (fg);
	std::string STRF = std::to_string(numF).c_str();
	const char* tempF;
	tempF = STRF.c_str();
	strcat(foreground, tempF);

    char *background=strdup(bgBase);
    //background[5]+=bg;
	int numB = static_cast<int> (bg);
    std::string STRB = std::to_string(numB).c_str();
    const char* tempB;
    tempB = STRB.c_str();
    strcat(background, tempB);
    // initialize the formatting string
    char formatStr[kFormatStrSize]="";
    // according to the options, append appropriate string
	//strcat(formatStr, init);
    if (hi) {
		strcat(formatStr, init);
		strcat(formatStr, hilit);
		formatStr[strlen(formatStr) - 1] = 'm';
	}
	if (blinking) {
		strcat(formatStr, init);
    	strcat(formatStr, blink);
		formatStr[strlen(formatStr) - 1] = 'm';
	}
    if (fg!=NOCHANGE) {
		strcat(formatStr, init);
        strcat(formatStr, foreground);
		strcat(formatStr,endc);
    }
    if (bg!=NOCHANGE) {
		strcat(formatStr, init);
        strcat(formatStr, background);
		strcat(formatStr,endc);
    }
    // terminate the options
    /*if (formatStr[strlen(formatStr)-1]==';')
    	formatStr[strlen(formatStr)-1]= '\0';
    strcat(formatStr,endc);*/


    free(background);
    free(foreground);

    std::string res;
#ifdef _WIN
    res.append(str)
#else
    res.append(formatStr);
    res.append(str);
    res.append(recover);
#endif
    return res;
}

/** 
 * This function takes a string and ansi formatting option such as 
 * hilighting for foreground color,  and blinking option and 
 * then send the appropriate string to the standard output. 
 * The two options are optional. So, when no options are given,
 * it prints the string as normal string.
 */

std::string
AnsiPrint(const char *str, bool hi, bool blinking) {

    // kick out exceptional case
    if ((str==NULL)||(strlen(str)==0))
        return "";

    char formatStr[kFormatStrSize]="";
    if (hi||blinking) {
        // initialize the formatting string
        strcat(formatStr, init);
        // according to the options, append appropriate string
        if (hi) {
            strcat(formatStr, hilit);
        }
        if (blinking) {
            strcat(formatStr, blink);
        }
        if (formatStr[strlen(formatStr)-1]==';')
            formatStr[strlen(formatStr)-1]= '\0';
        strcat(formatStr,endc);
    }

    std::string res;
#ifdef _WIN
    res.append(str)
#else
    res.append(formatStr);
    res.append(str);
    res.append(recover);
#endif

    return res;


}

std::string
AnsiPrint(const char *str, int fg, int bg, bool hi, bool blinking) {

    // kick out exceptional case
    if ((str==NULL)||(strlen(str)==0))
        return "";
    // creating foreground and background options
    char *foreground=strdup(fgBase);
    //foreground[5]+=fg;
        std::string STRF = std::to_string(fg).c_str();
        const char* tempF;
        tempF = STRF.c_str();
        strcat(foreground, tempF);

    char *background=strdup(bgBase);
    //background[5]+=bg;
    std::string STRB = std::to_string(bg).c_str();
    const char* tempB;
    tempB = STRB.c_str();
    strcat(background, tempB);
    // initialize the formatting string
    char formatStr[kFormatStrSize]="";
    // according to the options, append appropriate string
        //strcat(formatStr, init);
    if (hi) {
                strcat(formatStr, init);
                strcat(formatStr, hilit);
                formatStr[strlen(formatStr) - 1] = 'm';
        }
        if (blinking) {
                strcat(formatStr, init);
        strcat(formatStr, blink);
                formatStr[strlen(formatStr) - 1] = 'm';
        }
    if (fg!=NOCHANGE) {
                strcat(formatStr, init);
        strcat(formatStr, foreground);
                strcat(formatStr,endc);
    }
    if (bg!=NOCHANGE) {
                strcat(formatStr, init);
        strcat(formatStr, background);
                strcat(formatStr,endc);
    }
    // terminate the options
    /*if (formatStr[strlen(formatStr)-1]==';')
        formatStr[strlen(formatStr)-1]= '\0';
    strcat(formatStr,endc);*/


    free(background);
    free(foreground);

    std::string res;
#ifdef _WIN
    res.append(str)
#else
    res.append(formatStr);
    res.append(str);
    res.append(recover);
#endif
    return res;
}
