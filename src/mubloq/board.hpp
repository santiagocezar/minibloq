#include <string>
#include <immer/vector.hpp>

namespace hardware {
    // big-ass struct lfg
    struct board {
        std::string name;
        std::string path; // to the examples
        std::string imgMain; // reference ig
        std::string imgThumb; // the icon prolly
        
        std::string cpu; 
        std::string clockFreq;
        
        std::string url0; // to the main site
        std::string url1; // to something else if you want
        
        std::string portType; // no idea why does it need this, it's just HID or HID2
        
        std::string lang; // points to the toolchain
        
        std::string uploader; // the executable that uploads
        std::string uploaderCmd0; // for some reason the basename for uploader...
        
        std::string corePath; // the core of what
        std::string driverPath; // ok this is self-explanatory
        std::string core; // some kind of linkable object
        
        std::string outputMainFileExtension;
        
        bool resetBeforeBuild; // this is always false
        
        unsigned int bootBaudRate; // why the word "boot"
        unsigned int bootFindPortTries; // yep 15 everytime, why does it need to be configurable
        unsigned int bootTimeOut; // 200 here, idk
        
        // uhhh it's only used in src/Bubble/wxBubble/v1.0/Source/Bubble.cpp:2141
        // but currently the program never get's inside that branch so... yeah
        bool useWrapper;
        std::string headerFileExtension;
        std::string codeFileExtension;
        std::string includeCodePrefix;
        std::string includeCodePostfix;
        std::string includeCodeInstancePrefix;
        std::string includeInitCode;
        std::string includeFinalCode;
        std::string includeBuildPrefix;
        std::string includeBuildPostfix;
        std::string initBoardHeader;
        std::string initBoardPrefix;
        std::string initBoardPostfix;
        std::string commentBegin;
        std::string commentEnd;
        std::string includesCodeList;
        std::string definesCodeList;
        std::string instancesCodeList;
        std::string instancesHeaderCodeList;
        std::string initCode;
        std::string finalCode;
        std::string initBoardCode;
        std::string includesBuildList;
        std::string arduinoVersion; //This is specific to Arduino-compatible hardware, but needed by now.
        std::string objectExtension;
        std::string boardDefine;
        std::string arduinoVariant;
        std::string usbVid;
        std::string usbPidBoot;
        std::string usbPidApp;
        std::string usbManufacturer;
        std::string usbProduct;
        std::immer<std::string> relCommands;
        std::immer<std::string> fileExtensions;
        
        int codeLexer;
        wxColour codeOperatorColor;
        wxColour codeStringColor;
        wxColour codePreprocessorColor;
        wxColour codeIdentifierColor;
        wxColour codeNumberColor;
        wxColour codeCharacterColor;
        wxColour codeWordColor;
        wxColour codeWord2Color;
        wxColour codeCommentColor;
        wxColour codeCommentLineColor;
        wxColour codeCommentDocColor;
        wxColour codeCommentDocKeywordColor;
        wxColour codeCommentDocKeywordErrorColor;
        bool codeOperatorBold;
        bool codeStringBold;
        bool codePreprocessorBold;
        bool codeIdentifierBold;
        bool codeNumberBold;
        bool codeCharacterBold;
        bool codeWordBold;
        bool codeWord2Bold;
        bool codeCommentBold;
        bool codeCommentLineBold;
        bool codeCommentDocBold;
        bool codeCommentDocKeywordBold;
        bool codeCommentDocKeywordErrorBold;
        unsigned int codeTabWidth;
        std::string codeKeywords0;
        std::string codeKeywords1;
    }
}
