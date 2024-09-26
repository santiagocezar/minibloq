#include "xml_compat.hpp"

#include <optional>
#include <filesystem>
#include <wx/xml/xml.h>

#if defined (linux) && defined(__x86_64__)
    const std::string HOST_STR = "linux_amd64";
#else
    const std::string HOST_STR = "win32_amd64"; // TODO: change host strings in main.board
#endif

namespace hardware {

    // std::string get_magical_variable_value(const std::string &variableName, const std::string& fileName)
    // {
        /*
         * actually no idea how is this supposed to work, like, getInternalVariableValue
         * reads from the currentBoardProperties from the hardwareManager... which when
         * loading the hardware targets it's still being constructed and is probably
         * null...
         * - valsan
         */

        //Internal variables has priority over XML loaded variables:
        // wxString internalVariableValue = getInternalVariableValue(variableName, fileName);
        // if (internalVariableValue != "")
            // return internalVariableValue;

        /*
         * also main.board doesn't seem to use variables
         */

        // BubbleInstance *variable = XMLVariables.getInstance(variableName);
        // if (variable)
        //     return variable->getType(); //Here, we use the type field to store the value.

        /*
         * so yeah this does nothing, FIXME
         */

        // return "";
    // }


    void get_board_props(board &board, wxXmlNode *node) {
        wxXmlNode *child = node->GetChildren();
        while (child)
        {
            //##Debug:
            //wxMessageDialog dialog0(bubble->getParent(), wxString("child = ") + child->GetNodeContent(), boardInfo->getName()); //##Debug.
            //dialog0.ShowModal(); //##Debug.

            if (child->GetName() == "name") {
                board.name = child->GetNodeContent();
            } else if (child->GetName() == "portType") {
                board.portType = child->GetNodeContent();
                //##Debug:
                //wxMessageDialog dialog0(bubble->getParent(), wxString("portType = ") + child->GetNodeContent(), boardInfo->getName()); //##Debug.
                //dialog0.ShowModal(); //##Debug.
            } else if (child->GetName() == "imgMain") {
                board.imgMain = child->GetNodeContent();
            } else if (child->GetName() == "cpu") {
                board.cpu = child->GetNodeContent();
            } else if (child->GetName() == "clockFreq") {
                board.clockFreq = child->GetNodeContent();
            } else if (child->GetName() == "url0") {
                board.url0 = child->GetNodeContent();
            } else if (child->GetName() == "url1") {
                board.url1 = child->GetNodeContent();
            } else if (child->GetName() == "imgThumb") {
                board.imgThumb = child->GetNodeContent();
            } else if (child->GetName() == HOST_STR + "_lang") {
                //The lang, corePath and core fields are filled with the host compatible language and core,
                //where the host is detected by miniBloq at startup.
                board.lang = child->GetNodeContent();
            } else if (child->GetName() == HOST_STR + "_uploader") {
                board.uploader = child->GetNodeContent();
            } else if (child->GetName() == HOST_STR + "_uploaderCmd_0") {
                board.uploaderCmd0 = child->GetNodeContent();
            } else if (child->GetName() == HOST_STR + "_corePath") {
                board.corePath = child->GetNodeContent();

                //##Debug:
//                    wxMessageDialog dialog0(bubble->getParent(), bubble->getHost() + wxString("_corePath"), boardInfo->getName() +
//                                            wxString(": ") + boardInfo->getCorePath());
//                    dialog0.ShowModal(); //##Debug.
            } else if (child->GetName() == HOST_STR + "_core") {
                board.core = child->GetNodeContent();
            } else if (child->GetName() == HOST_STR + "_driverPath") {
                board.driverPath = child->GetNodeContent();
                //##Debug:
//                    wxMessageDialog dialog0(bubble->getParent(),
//                                            boardInfo->getName() + wxString(": ") + child->GetNodeContent(), //boardInfo->getDriverPath(),
//                                            bubble->getHost() + wxString("_driverPath")
//                                           );
//                    dialog0.ShowModal(); //##Debug.
            }
            else if (child->GetName() == "outputMainFileExtension") {
                board.outputMainFileExtension = child->GetNodeContent();
            } else if (child->GetName() == "resetBeforeBuild") {
                board.resetBeforeBuild = child->GetNodeContent() == "true";
            } else if (child->GetName() == "bootBaudRate") {
                child->GetNodeContent().ToUInt(&board.bootBaudRate);
            } else if (child->GetName() == "bootFindPortTries") {
                child->GetNodeContent().ToUInt(&board.bootFindPortTries);
            } else if (child->GetName() == "useWrapper") {
                board.useWrapper = child->GetNodeContent() == "true";
            } else if (child->GetName() == "bootTimeOut") {
                child->GetNodeContent().ToUInt(&board.bootTimeOut);
            } else if (child->GetName() == "headerFileExtension") {
                board.headerFileExtension = child->GetNodeContent();
            } else if (child->GetName() == "codeFileExtension") {
                board.codeFileExtension = child->GetNodeContent();
            } else if (child->GetName() == "includeCodePrefix") {
                board.includeCodePrefix = child->GetNodeContent();
            } else if (child->GetName() == "includeCodePostfix") {
                board.includeCodePostfix = child->GetNodeContent();
            } else if (child->GetName() == "includeCodeInstancePrefix") {
                board.includeCodeInstancePrefix = child->GetNodeContent();
            } else if (child->GetName() == "includeInitCode") {
                board.includeInitCode = child->GetNodeContent();
            } else if (child->GetName() == "includeFinalCode") {
                board.includeFinalCode = child->GetNodeContent();
            } else if (child->GetName() == "includeBuildPrefix") {
                board.includeBuildPrefix = child->GetNodeContent();
            } else if (child->GetName() == "includeBuildPostfix") {
                board.includeBuildPostfix = child->GetNodeContent();
            } else if (child->GetName() == "initBoardHeader") {
                board.initBoardHeader = child->GetNodeContent();
            } else if (child->GetName() == "initBoardPrefix") {
                board.initBoardPrefix = child->GetNodeContent();
            } else if (child->GetName() == "initBoardPostfix") {
                board.initBoardPostfix = child->GetNodeContent();
            } else if (child->GetName() == "commentBegin") {
                board.commentBegin = child->GetNodeContent();
            } else if (child->GetName() == "commentEnd") {
                board.commentEnd = child->GetNodeContent();
            } else if (child->GetName() == "arduinoVersion") {
                board.arduinoVersion = child->GetNodeContent();
            } else if (child->GetName() == "objectExtension") {
                board.objectExtension = child->GetNodeContent();
            } else if (child->GetName() == "boardDefine") {
                board.boardDefine = child->GetNodeContent();
            } else if (child->GetName() == "arduinoVariant") {
                board.arduinoVariant = child->GetNodeContent();
                //##Debug:
//                    wxMessageDialog dialog0(bubble->getParent(), boardInfo->getArduinoVariant(), boardInfo->getName()); //##Debug.
//                    dialog0.ShowModal(); //##Debug.
            } else if (child->GetName() == "usbVid") {
                board.usbVid = child->GetNodeContent();
            } else if (child->GetName() == "usbPidBoot") {
                board.usbPidBoot = child->GetNodeContent();
            } else if (child->GetName() == "usbPidApp") {
                board.usbPidApp = child->GetNodeContent();
            } else if (child->GetName() == "usbManufacturer") {
                board.usbManufacturer = child->GetNodeContent();
            } else if (child->GetName() == "usbProduct") {
                board.usbProduct = child->GetNodeContent();
            }

            child = child->GetNext();
        }
    }

    void get_syntax_config(board &board, wxXmlNode *node) {
        wxXmlNode *child = node->GetChildren();
        while (child)
        {
            if (child->GetName() == wxString("lexer")) {
                //Syntax lexer:
                child->GetNodeContent().ToInt(&board.codeLexer);
            } else if (child->GetName() == wxString("operatorColor")) {
                //Font color:
                board.codeOperatorColor = child->GetNodeContent();
            } else if (child->GetName() == wxString("stringColor")) {
                board.codeStringColor = child->GetNodeContent();
            } else if (child->GetName() == wxString("preprocesorColor")) {
                board.codePreprocessorColor = child->GetNodeContent();
            } else if (child->GetName() == wxString("identifierColor")) {
                board.codeIdentifierColor = child->GetNodeContent();
            } else if (child->GetName() == wxString("numberColor")) {
                board.codeNumberColor = child->GetNodeContent();
            } else if (child->GetName() == wxString("characterColor")) {
                board.codeCharacterColor = child->GetNodeContent();
            } else if (child->GetName() == wxString("wordColor")) {
    //            wxMessageDialog dialog0(bubble->getParent(), _(""), stringNode->GetNodeContent()); //##Debug
    //            dialog0.ShowModal(); //##Debug
                board.codeWordColor = child->GetNodeContent();
            } else if (child->GetName() == wxString("word2Color")) {
                board.codeWord2Color = child->GetNodeContent();
            } else if (child->GetName() == wxString("commentColor")) {
                board.codeCommentColor = child->GetNodeContent();
            } else if (child->GetName() == wxString("commentLineColor")) {
                board.codeCommentLineColor = child->GetNodeContent();
            } else if (child->GetName() == wxString("commentDocColor")) {
                board.codeCommentDocColor = child->GetNodeContent();
            } else if (child->GetName() == wxString("commentDocKeywordColor")) {
                board.codeCommentDocKeywordColor = child->GetNodeContent();
            } else if (child->GetName() == wxString("commentDocKeywordErrorColor")) {
                board.codeCommentDocKeywordErrorColor = child->GetNodeContent();
            } else if (child->GetName() == wxString("operatorBold")) {
                //Bold fonts:
                board.codeOperatorBold = child->GetNodeContent() == "true";
            } else if (child->GetName() == wxString("stringBold")) {
                board.codeStringBold = child->GetNodeContent() == "true";
            } else if (child->GetName() == wxString("preprocesorBold")) {
                board.codePreprocessorBold = child->GetNodeContent() == "true";
            } else if (child->GetName() == wxString("identifierBold")) {
                board.codeIdentifierBold = child->GetNodeContent() == "true";
            } else if (child->GetName() == wxString("numberBold")) {
                board.codeNumberBold = child->GetNodeContent() == "true";
            } else if (child->GetName() == wxString("characterBold")) {
                board.codeCharacterBold = child->GetNodeContent() == "true";
            } else if (child->GetName() == wxString("wordBold")) {
                board.codeWordBold = child->GetNodeContent() == "true";
            } else if (child->GetName() == wxString("word2Bold")) {
                board.codeWord2Bold = child->GetNodeContent() == "true";
            } else if (child->GetName() == wxString("commentBold")) {
                board.codeCommentBold = child->GetNodeContent() == "true";
            } else if (child->GetName() == wxString("commentLineBold")) {
                board.codeCommentLineBold = child->GetNodeContent() == "true";
            } else if (child->GetName() == wxString("commentDocBold")) {
                board.codeCommentDocBold = child->GetNodeContent() == "true";
            } else if (child->GetName() == wxString("commentDocKeywordBold")) {
                board.codeCommentDocKeywordBold = child->GetNodeContent() == "true";
            } else if (child->GetName() == wxString("commentDocKeywordErrorBold")) {
                board.codeCommentDocKeywordErrorBold = child->GetNodeContent() == "true";
            } else if (child->GetName() == wxString("tabWidth")) {
                child->GetNodeContent().ToUInt(&board.codeTabWidth);
            }

            child = child->GetNext();
        }
    }

    void get_file_extensions(board &board, wxXmlNode *node) {
        wxXmlNode *child = node->GetChildren();
        std::vector<std::string> fileExtensions;
        while (child)
        {
            fileExtensions.push_back(child->GetName().ToStdString());
            child = child->GetNext();
        }
        board.fileExtensions = immer::vector<std::string>(fileExtensions.begin(), fileExtensions.end());
    }

    // FIXME: make this stateless
    void get_include_files(board &board, wxXmlNode *node, bool only_board) {
        wxXmlNode *child = node->GetChildren();
        std::string include_str;
        while (child) {
            std::string result;
            if (child->GetName() == wxString("include")) {
                wxXmlNode *include_child = child->GetChildren();
                while (include_child) {
                    if (include_child->GetName() == wxString("s")) {
                        std::string stringLine = include_child->GetNodeContent().ToStdString();

                        //Is the value a variable?
                        // if (isXMLVariable(stringLine))
                            // stringLine = getVariableValue(stringLine, wxString(""));

                        result += stringLine;
                    }
                    include_child = include_child->GetNext();
                }
            }
            child = child->GetNext();

            //Is a board include or a relation include?
            if (only_board)
            {
                //Board:
                board.includesCodeList = board.includesCodeList +
                                         board.includeCodePrefix + result +
                                         board.includeCodePostfix + "\n";
            }
            /* no else here?
            {
                //Relation:
                bubble->setIncludesCodeList(bubble->getIncludesCodeList() +
                                            boardProperties->getIncludeCodePrefix() + resultStr +
                                            boardProperties->getIncludeCodePostfix() + wxString("\r\n")
                                        );
            }
            */
        }
    }

    // BubbleXML::loadIncludePathsFromXML
    void get_include_paths(board &board, wxXmlNode *node, bool onlyBoard) {
        wxXmlNode *child = node->GetChildren();
        wxString includeStr("");
        while (child)
        {
            wxString resultStr("");
            if (child->GetName() == wxString("include"))
            {
                wxXmlNode *stringNode = child->GetChildren();
                while (stringNode)
                {
                    if (stringNode->GetName() == wxString("s"))
                    {
                        wxString stringLine = stringNode->GetNodeContent();

                        //Is the value a variable?
                        if (isXMLVariable(stringLine))
                            stringLine = getVariableValue(stringLine, wxString(""));

                        resultStr = resultStr + stringLine;
                    }
                    stringNode = stringNode->GetNext();
                }
            }
            child = child->GetNext();


            //Is a board include or a relation include?
            if (onlyBoard)
            {
                //Board:
                boardProperties->setIncludesBuildList(boardProperties->getIncludesBuildList() +
                                                boardProperties->getIncludeBuildPrefix() + resultStr +
                                                boardProperties->getIncludeBuildPostfix()
                                            );
            }
            {
                //Relation:
                bubble->setIncludesBuildList(bubble->getIncludesBuildList() +
                                                boardProperties->getIncludeBuildPrefix() + resultStr +
                                                boardProperties->getIncludeBuildPostfix()
                                            );
            }
        }
    }

    // BubbleXML::loadInitBoardCodeFromXML
    void get_init_board_code(board &board, wxXmlNode *node, bool onlyBoard) {
        wxXmlNode *stringNode = node->GetChildren();
        while (stringNode)
        {
            if (stringNode->GetName() == wxString("s"))
            {
                wxString stringLine = stringNode->GetNodeContent();

                //Is the value a variable?
                if (isXMLVariable(stringLine))
                    stringLine = getVariableValue(stringLine, wxString(""));

                //Is a board include or a relation include?
                if (onlyBoard)
                {
                    //Board:
                    boardProperties->setInitBoardCode(boardProperties->getInitBoardCode() + stringLine);
                }
                {
                    //Relation:
                    bubble->setInitBoardCode(bubble->getInitBoardCode() + stringLine);
                }
            }
            stringNode = stringNode->GetNext();
        }
    }

    // BubbleXML::loadInitCodeFromXML
    void get_init_code(board &board, wxXmlNode *node) {
        wxXmlNode *stringNode = node->GetChildren();
        while (stringNode)
        {
            if (stringNode->GetName() == wxString("s"))
            {
                wxString stringLine = stringNode->GetNodeContent();

                //Is the value a variable?
                if (isXMLVariable(stringLine))
                    stringLine = getVariableValue(stringLine, wxString(""));

                boardProperties->setInitCode(boardProperties->getInitCode() + stringLine);
            }
            stringNode = stringNode->GetNext();
        }
    }

    // BubbleXML::loadFinalCodeFromXML
    void get_final_code(board &board, wxXmlNode *node) {
        std::string result("");
        wxXmlNode *stringNode = node->GetChildren();
        while (stringNode)
        {
            if (stringNode->GetName() == wxString("s"))
            {
                result = stringNode->GetNodeContent();// + wxString("\r\n") ;
                board.finalCode += result;
            }
            stringNode = stringNode->GetNext();
        }
    }

    // TODO: damn xml, this should just be a json (or yaml)
    // BubbleXML::loadBoardProperties
    std::optional<board> load_main_board(std::filesystem::path path) {
        const auto& main_board = path / "main.board";

        if (!std::filesystem::exists(main_board))
            return std::nullopt;

        //##Future: Try to disable the error messages that the wxXmlDocument class fires when encounters errors:
        wxXmlDocument boardFile;
        if (!boardFile.Load(main_board.c_str(), "UTF-8"))
            return std::nullopt;

        wxXmlNode *root = boardFile.GetRoot();
        if (!root)
            return std::nullopt;

        if (root->GetName() != wxString("board"))
            return std::nullopt;

        board board;

        wxString tempName("");
        wxXmlNode *rootChild = root->GetChildren();
        while (rootChild) {
            tempName = rootChild->GetName();
            if (tempName == wxString("properties")) {
                get_board_props(board, rootChild);
            } else if (tempName == wxString("syntax")) {
                get_syntax_config(board, rootChild);
            } else if (tempName == wxString("examples")) {
                // TODO: implementing this
            }
            else if (tempName == wxString("fileExtensions")) {
                get_file_extensions(board, rootChild);
            } else if (tempName == wxString("includeFiles")) {
                get_include_files(board, rootChild, true);
                //wxMessageDialog dialog0(bubble->getParent(), bubble->getIncludesCodeList(), boardInfo->getName()); //##Debug.
                //dialog0.ShowModal(); //##Debug.
            } else if (tempName == wxString("includePaths")) {
                get_include_paths(board, rootChild, true);
            } else if (tempName == wxString("initBoardCode")) {
                get_init_board_code(board, rootChild, true);
            } else if (tempName == wxString("initCode")) {
                get_init_code(board, rootChild);
                // loadInitCodeFromXML(rootChild, boardInfo);
            } else if (tempName == wxString("finalCode")) {
                get_final_code(board, rootChild);
            }

            rootChild = rootChild->GetNext();
        }
        //wxMessageDialog dialog0(bubble->getParent(), boardInfo->getLang(), boardInfo->getName()); //##Debug.
        //wxMessageDialog dialog0(bubble->getParent(), bubble->getHost() + wxString("_lang"), boardInfo->getName()); //##Debug.
        //dialog0.ShowModal(); //##Debug.

        return board;
    }

    // BubbleXML::loadHardwareTargets
    std::vector<board> load_all_boards(std::string path) {
        std::vector<board> boards;
        for (const auto & entry : std::filesystem::directory_iterator(path)) {
            auto board = load_main_board(entry.path());
            if (board) {
                boards.push_back(*board);
            }
        }
        return boards;
    }
}
