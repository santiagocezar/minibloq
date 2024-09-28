#pragma once

#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/colour.h>

class BubbleBoardProperties
{
    public:
        wxString name;
        wxString path;
        wxString imgMain;
        wxString cpu;
        wxString clockFreq;
        wxString url0;
        wxString url1;
        wxString imgThumb;
        wxString portType;
        wxString lang;
        wxString uploader;
        wxString uploaderCmd0;
        wxString corePath;
        wxString driverPath;
        wxString core;
        wxString outputMainFileExtension;
        bool resetBeforeBuild;
        unsigned int bootBaudRate;
        unsigned int bootFindPortTries;
        unsigned int bootTimeOut;
        bool useWrapper;
        wxString headerFileExtension;
        wxString codeFileExtension;
        wxString includeCodePrefix;
        wxString includeCodePostfix;
        wxString includeCodeInstancePrefix;
        wxString includeInitCode;
        wxString includeFinalCode;
        wxString includeBuildPrefix;
        wxString includeBuildPostfix;
        wxString initBoardHeader;
        wxString initBoardPrefix;
        wxString initBoardPostfix;
        wxString commentBegin;
        wxString commentEnd;
        wxString includesCodeList;
        wxString definesCodeList;
        wxString instancesCodeList;
        wxString instancesHeaderCodeList;
        wxString initCode;
        wxString finalCode;
        wxString initBoardCode;
        wxString includesBuildList;
        wxString arduinoVersion; //This is specific to Arduino-compatible hardware, but needed by now.
        wxString objectExtension;
        wxString boardDefine;
        wxString arduinoVariant;
        wxString usbVid;
        wxString usbPidBoot;
        wxString usbPidApp;
        wxString usbManufacturer;
        wxString usbProduct;
        wxArrayString relCommands;
        wxArrayString fileExtensions;

        int codeLexer;
        wxColor codeOperatorColor;
        wxColor codeStringColor;
        wxColor codePreprocessorColor;
        wxColor codeIdentifierColor;
        wxColor codeNumberColor;
        wxColor codeCharacterColor;
        wxColor codeWordColor;
        wxColor codeWord2Color;
        wxColor codeCommentColor;
        wxColor codeCommentLineColor;
        wxColor codeCommentDocColor;
        wxColor codeCommentDocKeywordColor;
        wxColor codeCommentDocKeywordErrorColor;
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
        wxString codeKeywords0;
        wxString codeKeywords1;

        BubbleBoardProperties():    portType("serial"),
                                    outputMainFileExtension("ino"), //##Arduino compatible file by default?
                                    bootBaudRate(115200),
                                    bootFindPortTries(5),
                                    bootTimeOut(200),
                                    codeLexer(3), //CPP syntax
                                    codeTabWidth(4)
        {}
        //##Ver si se necesita constructor de copia por las dudas, al menos que no haga gran cosa...

        virtual ~BubbleBoardProperties()
        {
        }

        // inline void clearRelCommands() { return relCommands.Clear(); };
        // inline unsigned int getRelCommandsCount() const { return relCommands.GetCount(); };
        // inline void addRelCommand(const wxString& value)
        // {
        //     relCommands.Add(value);
        // }
        // inline const wxString getRelCommand(const unsigned int index) const
        // {
        //     if (index < relCommands.GetCount())
        //         return relCommands[index];
        //     return wxString("");
        // }

        // inline void clearFileExtensions() { return fileExtensions.Clear(); };
        // inline unsigned int getFileExtensionsCount() const { return fileExtensions.GetCount(); };
        // inline void addFileExtension(const wxString& value)
        // {
        //     fileExtensions.Add(value);
        // }
        // inline const wxString getFileExtension(const unsigned int index) const
        // {
        //     if (index < fileExtensions.GetCount())
        //         return fileExtensions[index];
        //     return wxString("");
        // }

        // inline void setCodeOperatorColor(const wxString& value) { codeOperatorColor = string2color(value); }
        // inline void setCodeOperatorColor(const wxColour& value) { codeOperatorColor = value; }

        inline void addCodeKeyWord(wxString& keywords, const wxString& value)
        {
            if (!keywords.Contains(value))
                keywords += wxString(" ") + value;
        }

        void addCodeKeywords0(const wxString& value) { addCodeKeyWord(codeKeywords0, value); }
        void addCodeKeywords1(const wxString& value) { addCodeKeyWord(codeKeywords1, value); }
};
