#pragma once

#include <string>
#include <immer/vector.hpp>
#include <variant>
#include <array>

namespace hardware
{
    typedef std::array<uint8_t, 4> color;

    // big-ass struct lfg
    // TODO: probably split this into multiple structs, make the XML loader stateless.
    struct board {
        std::string name;
        std::string path; // to the examples
        std::string imgMain; // reference ig
        std::string imgThumb; // the icon prolly

        std::string cpu;
        std::string clockFreq;

        std::string url0; // to the main site
        std::string url1; // to something else if you want

        std::string portType = "serial";

        std::string lang; // points to the toolchain

        std::string uploader; // the executable that uploads
        std::string uploaderCmd0; // for some reason the basename for uploader...

        std::string corePath; // the core of what
        std::string driverPath; // ok this is self-explanatory
        std::string core; // some kind of linkable object

        std::string outputMainFileExtension = "ino";

        bool resetBeforeBuild; // this is always false

        unsigned int bootBaudRate = 115200; // why the word "boot"
        unsigned int bootFindPortTries = 5;
        unsigned int bootTimeOut = 200; // 200 here, idk

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

        immer::vector<std::string> relCommands;
        immer::vector<std::string> fileExtensions;

        int codeLexer = 3;

        // TODO: use a color type
        std::string codeOperatorColor;
        std::string codeStringColor;
        std::string codePreprocessorColor;
        std::string codeIdentifierColor;
        std::string codeNumberColor;
        std::string codeCharacterColor;
        std::string codeWordColor;
        std::string codeWord2Color;
        std::string codeCommentColor;
        std::string codeCommentLineColor;
        std::string codeCommentDocColor;
        std::string codeCommentDocKeywordColor;
        std::string codeCommentDocKeywordErrorColor;

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
        unsigned int codeTabWidth = 4;
        std::string codeKeywords0;
        std::string codeKeywords1;
    };

    struct model {
        immer::vector<board> boards;
        size_t selected_board;
        immer::vector<std::string> available_ports;
        int selected_port = -1;
    };

    struct reload_ports {};
    struct select_port {
        int idx;
    };

    struct load_hardware {
        std::string path;
    };
    
    using actions = std::variant<
        reload_ports,
        select_port,
        load_hardware
    >;

    model update(model m, actions a);
} // namespace hardware
