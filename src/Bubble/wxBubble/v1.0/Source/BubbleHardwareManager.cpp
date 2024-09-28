#include "BubbleHardwareManager.h"
#include "Bubble.h"
#include "mubloq/hardware.hpp"

#if defined (WIN32)
    #include <windows.h>
#endif

#include "MainFrame.h"
#include <wx/dir.h>
#include <fmt/core.h>


BEGIN_EVENT_TABLE(BubbleHardwareManager, BubblePanel)
    EVT_SIZE(BubbleHardwareManager::onSize)
END_EVENT_TABLE()


BubbleHardwareManager::BubbleHardwareManager(   wxWindow* parent,
                                                wxWindowID id,
                                                lager::reader<hardware::model> hardware,
                                                lager::context<hardware::actions> ctx,
                                                Bubble *const bubble,
                                                const wxString& boardName,
                                                const wxPoint& pos,
                                                const wxSize& size,
                                                long style,
                                                const wxString& name) : wxPanel(parent,
                                                                                id,
                                                                                pos,
                                                                                size,
                                                                                style,
                                                                                name),
                                                                        hardware(hardware),
                                                                        ports(hardware[&hardware::model::available_ports]),
                                                                        selected_port(hardware[&hardware::model::selected_port]),
                                                                        ctx(ctx),
                                                                        parent(parent),
                                                                        bubble(bubble),
                                                                        boardName(boardName),
                                                                        currentBoardProperties(NULL),
                                                                        lblBootPortName(NULL),
                                                                        comboBootPortName(NULL),
                                                                        lblBoardName(NULL),
                                                                        comboBoardName(NULL),
                                                                        buttonReloadBlocks(NULL),
                                                                        buttonReloadHardware(NULL),
                                                                        lblURL0(NULL),
                                                                        lblURL1(NULL),
                                                                        buttonGoToDriversDir(NULL),
                                                                        buttonMainImage(NULL),
                                                                        emptyDummyString(wxString(""))
{
    if (bubble == NULL)
        return; //Nothing to do.

    currentBoardProperties = new BubbleBoardProperties();

    //Hide();

    //##The following line crashes the application when the BubbleHardwareManager is closed by the user, so
    //##don't delete it, because it may be possibly a wxAUI's bug, and I would like to document it:
    //SetSize(400, 450);

    lblBoardName = new wxStaticText(   this,
                                        wxNewId(), //##
                                        //##Future: Make configurable by the user if he want to see these
                                        //shorcuts on screen or not:
                                        _("Hardware (Alt+W):"), //##Load shorcuts from XML.
                                        wxPoint(10, 10),
                                        wxSize(150, 20),
                                        wxALIGN_CENTRE,
                                        wxString("lblBoardName")
                                   );

    comboBoardName = new BubbleCombo(   this,
                                        wxNewId(),
                                        wxImage(bubble->getThemePath() + wxString("/ComboSelectDefault.png")),
                                        wxImage(bubble->getThemePath() + wxString("/ComboSelectPressed.png")),
                                        wxImage(bubble->getThemePath() + wxString("/ComboSelectHover.png")),
                                        wxImage(bubble->getThemePath() + wxString("/ComboSelectDisabled.png")),
                                        false,
                                        wxPoint(10, 25), //##Make this automatic.
                                        wxSize(190, 16),
                                        //wxTAB_TRAVERSAL|
                                        //wxNO_BORDER|
                                        //wxBORDER_SIMPLE|
                                        //wxBORDER_RAISED|
                                        wxBORDER_SUNKEN|
                                        wxTRANSPARENT_WINDOW,
                                        //wxDefaultSize,
                                        //##wxALIGN_CENTRE,
                                        wxString("comboBoardName")
                                    );
    if (comboBoardName)
    {
        comboBoardName->setSorted(false);
        bubble->loadHardwareTargets(this);


        if (!boardsProperties.empty()) {
            currentBoardProperties = &boardsProperties[0]; // TODO: check for use after free
            comboBoardName->setSelection(0);
        }

        if (boardName != wxString(""))
        {
            for (const auto &board : boardsProperties)
            {
                if (board.name == boardName)
                {
                    currentBoardProperties = &board;
                    comboBoardName->setSelection(boardName);
                    break;
                }
            }
        }

        comboBoardName->Connect(wxEVT_COMMAND_TEXT_UPDATED,
                                wxCommandEventHandler(BubbleHardwareManager::onComboBoardNameChanged),
                                NULL,
                                this
                               );
    }

    lblBootPortName = new wxStaticText( this,
                                        wxNewId(),
                                        //In the future there may be IP sockets, and other comm
                                        //channels, so here it's just "Port", not "Serial port":

                                        //##Future: Make configurable by the user if he want to see these
                                        //shorcuts on screen or not:
                                        _("Port (Alt+R):"), //##Load shorcuts from XML.
                                        wxPoint(210, 10),   //##Un-hardcode!
                                        wxSize(150, 20),    //##Un-hardcode!
                                        wxALIGN_CENTRE,
                                        wxString("lblBootSerialPortName")
                                       );

    comboBootPortName = new wxComboBox( this,
                                        wxNewId(),
                                        "",
                                        wxPoint(210, 25)   //##Un-hardcode!
                                       );
    if (comboBootPortName)
    {
        comboBootPortName->Connect( wxEVT_COMMAND_TOOL_CLICKED,
                                    wxCommandEventHandler(BubbleHardwareManager::onUpdatePorts),
                                    NULL,
                                    this
                                  );
        comboBootPortName->Connect( wxEVT_COMMAND_TEXT_UPDATED,
                                    wxCommandEventHandler(BubbleHardwareManager::onComboBootPortNameChanged),
                                    NULL,
                                    this
                                  );
        
        
        ports.watch([&](decltype (hardware::model::available_ports) ports) {
            comboBootPortName->Clear();
            for (auto &port: ports) {
                comboBootPortName->Append(port);
            }
        });
        
        selected_port.watch([&](int selected) {
            fmt::println("selected: {}", selected);
            comboBootPortName->SetSelection(selected);
        });
        
        updatePorts();
        setPortType();

        if (getCurrentBoardProperties())
        {
            //This is necessary for the first time the system loads the boards:
            if ( (getCurrentBoardProperties()->portType == wxString("HID")) ||
                 (getCurrentBoardProperties()->portType == wxString("HID2"))
               )
            {
                setPortSelectorEnabled(false);
                setPortNameString((getCurrentBoardProperties())->portType);
            }
        }
//        comboBootPortName->setSelection(0); //##Un-hardcode and get the port from the config file...
//        wxString strCommRealName = wxString("//./") + comboBootPortName->getText();
//        bubble->setBootPortName(strCommRealName);
    }

    buttonReloadBlocks = new wxButton(  this,
                                        wxNewId(),
                                        _("Reload blocks"),
                                        wxPoint(10, 55), //##Un-hardcode!
                                        //wxDefaultSize,
                                        wxSize(160, 25), //##Future: The autosize (with the label) feature does now work. Make it work!
                                        wxBU_EXACTFIT
                                     ); //##

    if (buttonReloadBlocks)
    {
        buttonReloadBlocks->Connect(wxEVT_LEFT_UP,
                                    wxMouseEventHandler(BubbleHardwareManager::onButtonReloadBlocksLeftUp),
                                    NULL,
                                    this
                                   );
        //buttonReloadBlocks->Hide(); //##Testing.
//        wxMessageDialog dialog0(bubble->getParent(), bubble->getProfile(), wxString("profile")); //##Debug.
//        dialog0.ShowModal(); //##Debug.
//        buttonReloadBlocks->Show(bubble->getProfile() == wxString("developer"));
    }

    buttonReloadHardware = new wxButton(  this,
                                        wxNewId(),
                                        _("Reload hardware"),
                                        wxPoint(175, 55), //##Un-hardcode!
                                        //wxDefaultSize,
                                        wxSize(160, 25), //##Future: The autosize (with the label) feature does now work. Make it work!
                                        wxBU_EXACTFIT
                                     ); //##

    if (buttonReloadHardware)
    {
        buttonReloadHardware->Connect(wxEVT_LEFT_UP,
                                    wxMouseEventHandler(BubbleHardwareManager::onButtonButtonReloadHardwareLeftUp),
                                    NULL,
                                    this
                                   );
        //buttonReloadHardware->Show(bubble->getProfile() == wxString("developer"));
    }

    buttonGoToDriversDir = new wxButton(  this,
                                        wxNewId(),
                                        _("Open folder with drivers"),
                                        wxPoint(10, 85), //##Un-hardcode!
                                        //wxDefaultSize,
                                        wxSize(160, 25), //##Future: autosize.
                                        wxBU_EXACTFIT
                                     );
    if (buttonGoToDriversDir)
    {
        buttonGoToDriversDir->Connect(  wxEVT_LEFT_UP,
                                        wxMouseEventHandler(BubbleHardwareManager::onButtonGoToDriversDirLeftUp),
                                        NULL,
                                        this
                                     );

        if (getCurrentBoardProperties())
        {
            buttonGoToDriversDir->Enable(getCurrentBoardProperties()->driverPath != wxString(""));
            lblURL0 = new wxHyperlinkCtrl(  this,
                                            wxNewId(),
                                            getCurrentBoardProperties()->url0,
                                            getCurrentBoardProperties()->url0,
                                            wxPoint(buttonGoToDriversDir->GetPosition().x,
                                                    buttonGoToDriversDir->GetPosition().y +
                                                    buttonGoToDriversDir->GetSize().GetHeight() + 10),
                                            wxDefaultSize,
                                            wxHL_ALIGN_LEFT
                                         );
            if (lblURL0)
            {
                lblURL0->SetVisitedColour(lblURL0->GetNormalColour());
                //wxHyperlinkCtrl Bug: the first time, it should be created big.
                if (buttonReloadHardware)
                {
                    lblURL0->SetSize(wxSize(buttonReloadHardware->GetPosition().x +
                                            buttonReloadHardware->GetSize().GetWidth() -
                                            buttonGoToDriversDir->GetPosition().x,
                                            lblURL0->GetSize().GetHeight()
                                           )
                                    );
                }
                lblURL1 = new wxHyperlinkCtrl(  this,
                                                wxNewId(),
                                                getCurrentBoardProperties()->url1,
                                                getCurrentBoardProperties()->url1,
                                                wxPoint(lblURL0->GetPosition().x,
                                                        lblURL0->GetPosition().y +
                                                        lblURL0->GetSize().GetHeight() + 10),
                                                wxDefaultSize,
                                                wxHL_ALIGN_LEFT
                                             );
                if (lblURL1)
                {
                    lblURL1->SetVisitedColour(lblURL1->GetNormalColour());
                    if (lblURL0)
                        lblURL1->SetSize(lblURL0->GetSize());
                }
            }
        }
    }

    // buttonMainImage = new BubbleButton( this,
    //                                     wxNewId(),
    //                                     wxPoint(0, 0),  //##Un-hardcode!
    //                                     wxSize(32, 32)  //##Un-hardcode!
    //                                   );
    //##buttonIcon->SetValidator(wxDefaultValidator);

    changeImage();

    //SetBackgroundColour(colour);
}


BubbleHardwareManager::~BubbleHardwareManager()
{
}

void BubbleHardwareManager::addBoard(BubbleBoardProperties boardProperties)
{
    if (comboBoardName)
    {
        boardsProperties.push_back(boardProperties);
        comboBoardName->append( boardProperties.name,
                                new wxImage(boardProperties.path + wxString("/img/") +
                                            boardProperties.imgThumb)
                                );
    }
}


void BubbleHardwareManager::updateGUI()
{
    if (lblBoardName)
        lblBoardName->SetLabel(_("Hardware (Alt+W):"));
    if (lblBootPortName)
        lblBootPortName->SetLabel(_("Port (Alt+R):"));

    if (buttonReloadBlocks)
        buttonReloadBlocks->SetLabel(_("Reload blocks"));
}


void BubbleHardwareManager::changeImage()
{
    if (buttonMainImage == NULL)
        return;
    if (currentBoardProperties == NULL)
        return;

    buttonMainImage->setImageDefault(wxImage(currentBoardProperties->path +
                                             wxString("/img/") + currentBoardProperties->imgMain)
                                    );
    fit(GetSize());

    //This works better, because just refreshing the buttonMainImage sometimes redraws the combos without
    //their border:
    Update();
    Refresh();
    //buttonMainImage->Update();
    //buttonMainImage->Refresh();
}



void BubbleHardwareManager::onButtonButtonReloadHardwareLeftUp(wxMouseEvent& event)
{
    if (bubble)
    {
        if (parent)
        {
            if (comboBoardName && comboBootPortName)
            {
                wxString selectedHardware = comboBoardName->getText();
                wxString selectedPort = comboBootPortName->GetValue();
                bubble->loadHardwareTargets(this);
                comboBoardName->setSelection(selectedHardware);
                // comboBootPortName->setSelection(selectedPort);
            }
        }
    }
}


void BubbleHardwareManager::onButtonGoToDriversDirLeftUp(wxMouseEvent& event)
{
    if (bubble)
    {
        if (parent)
        {
            if (buttonGoToDriversDir)
            {
                if (getCurrentBoardProperties())
                {
                    wxString folderPath =   bubble->getComponentsRepositoryPath() + wxString("/") +
                                            getCurrentBoardProperties()->driverPath;
//                    //##Debug:
//                    wxMessageDialog dialog0(bubble->getParent(), folderPath,
//                                            getCurrentBoardProperties()->getName()); //##Debug.
//                    dialog0.ShowModal(); //##Debug.
                    if (wxDir::Exists(folderPath))
                    {
                        if (wxLaunchDefaultApplication(folderPath))
                            return;
                    }
                    //##
                    //((MainFrame*)parent)->showMessage(  _("Can't find folder ") + folderPath,
                    //                                    true,
                    //                                    true,
                    //                                    *wxRED
                    //                                 );
                }
            }
        }
    }
}


void BubbleHardwareManager::onButtonReloadBlocksLeftUp(wxMouseEvent& event)
{
    if (bubble)
    {
        if (parent)
        {
//            if (buttonReloadBlocks)
//            {
//                buttonReloadBlocks->Update();
//                buttonReloadBlocks->Refresh();
//            }
            bubble->enableAllBlocks(false);
            bubble->loadBlocksInfo(parent, true);
            bubble->enableAllBlocks(true);
        }
    }
}


void BubbleHardwareManager::onSize(wxSizeEvent& event)
{
    // fit(event.GetSize());
    // Update();
    // Refresh();
//    if (comboBootPortName)
//    {
//        //comboBootPortName->Raise();
//    }
    // event.Skip();
}

void BubbleHardwareManager::fit(const wxSize& size)
{
    //##Crear un panel para alojar al comboBox del port, y que la imagen quede abajo: Además, armar todo esto
    //con un flexGrid o algo por el estilo:
    if (buttonMainImage)
    {
        //Margins:
        float margin = 0.025;
        int iconX = (int)(margin*size.GetWidth());
        int iconY = (int)(margin*size.GetHeight());
        if (comboBootPortName)
            iconY += comboBootPortName->GetSize().GetHeight()*2;

        int iconW = size.GetWidth() - 2*iconX;;
        int iconH = 0;

        //First, it tries to maximize witdh (including margins), and then calculates the corresponding height:
        if (buttonMainImage->getImageDefault().GetHeight() > 0)
        {
            float ratio = ((float)buttonMainImage->getImageDefault().GetHeight()/(float)buttonMainImage->getImageDefault().GetWidth());
            iconH = (int)(ratio*iconW);
        }

        //If the height is too large, maximizes height (including margins) to the available space, and recalculates the width:
        if ((iconH + 2*iconY) > (size.GetHeight() - 75)) //##
        {
            iconH = size.GetHeight() - 75 - 2*iconY;
            if (buttonMainImage->getImageDefault().GetWidth() > 0)
            {
                float ratio = ((float)buttonMainImage->getImageDefault().GetWidth()/(float)buttonMainImage->getImageDefault().GetHeight());
                iconW = (int)(ratio*iconH);
            }
        }

        //Finally, centers and scales the image:
        iconX = (size.GetWidth() / 2) - (iconW / 2);
//        if (buttonGoToDriversDir)
//        {
//            //20 is a hardcoded small margin:
//            iconY = buttonGoToDriversDir->GetPosition().y + buttonGoToDriversDir->GetSize().GetHeight() + 20;
//        }
        iconY = (size.GetHeight() / 2) - (iconH / 2);
        if (lblURL0)
        {
            //20 is a hardcoded small margin:
            iconY = lblURL0->GetPosition().y + lblURL0->GetSize().GetHeight() + 20;
        }
        if (lblURL1)
        {
            //20 is a hardcoded small margin:
            iconY = lblURL1->GetPosition().y + lblURL1->GetSize().GetHeight() + 20;
        }
        buttonMainImage->Move(iconX, iconY); //First moves the buttonMethod.
        buttonMainImage->SetSize(iconW, iconH);
        //buttonMainImage->Lower();
    }
}


//##@# Possible non-portable code:
bool BubbleHardwareManager::serialPortExists(const wxString& strPort)
{
#if defined (WIN32)
    HANDLE hFile = ::CreateFile(strPort.c_str(), GENERIC_READ |
                                GENERIC_WRITE, 0, NULL,
                                OPEN_EXISTING, 0, NULL);
    bool result = (hFile != INVALID_HANDLE_VALUE);
    CloseHandle(hFile);
    return result;
#else
    return true;
#endif
}


void BubbleHardwareManager::updatePorts()
{
    ctx.dispatch(hardware::reload_ports {});
}


//##
//void BubbleHardwareManager::getPorts(std::vector<std::string>& result)
//{
//    result = ports;
//}


void BubbleHardwareManager::setAllEnabled(bool value)
{
    if (comboBootPortName)
        comboBootPortName->Enable(value);
    if (comboBoardName)
        comboBoardName->Enable(value);
    if (buttonReloadBlocks)
        buttonReloadBlocks->Enable(value);
    if (buttonReloadHardware)
        buttonReloadHardware->Enable(value);
    if (buttonGoToDriversDir)
        buttonGoToDriversDir->Enable(value);
}


void BubbleHardwareManager::showDeveloperButtons(bool value)
{
    if (buttonReloadBlocks)
        buttonReloadBlocks->Show(value);
    if (buttonReloadHardware)
        buttonReloadHardware->Show(value);
}


void BubbleHardwareManager::setPortSelectorEnabled(bool value)
{
    if (comboBootPortName)
        comboBootPortName->Enable(value);
}


bool BubbleHardwareManager::getPortSelectorEnabled()
{
    if (comboBootPortName)
        return comboBootPortName->IsEnabled();
    return false;
}


wxString BubbleHardwareManager::getPortNameString()
{
    if (comboBootPortName)
        return comboBootPortName->GetValue();
    return emptyDummyString;
}


void BubbleHardwareManager::setPortNameString(const wxString& value)
{
    fmt::println("setPortNameString({})", value.utf8_string());
    if (comboBootPortName)
       comboBootPortName->SetValue(value);
}


void BubbleHardwareManager::setBoardSelection(const wxString& value)
{
    if (comboBoardName)
        comboBoardName->setSelection(value);
}


wxString BubbleHardwareManager::getBoardSelection()
{
    if (comboBoardName)
        return comboBoardName->getText();
    return emptyDummyString;
}


void BubbleHardwareManager::setPortSelection(const wxString& value)
{
    fmt::println("setPortSelection({})", value.utf8_string());
    if (comboBootPortName)
        comboBootPortName->SetValue(value);
}


wxString BubbleHardwareManager::getPortSelection()
{
    if (comboBootPortName)
        return comboBootPortName->GetValue();
    return wxEmptyString;
}


void BubbleHardwareManager::popUpPortList()
{
    if (IsShownOnScreen())
    {
        updatePorts();
        // if (comboBootPortName)
            // FIXME: comboBootPortName->popUpList();
    }
}


void BubbleHardwareManager::popUpBoardList()
{
    if (IsShownOnScreen())
    {
        if (comboBoardName)
            comboBoardName->popUpList();
    }
}


void BubbleHardwareManager::onUpdatePorts(wxCommandEvent &event)
{
    updatePorts();
    event.Skip();
}


void BubbleHardwareManager::setPortType()
{
    if (comboBootPortName)
    {
        if (bubble)
        {
            //##In the future this will be more flexible, allowing for TCP communications, among other:
            if (comboBootPortName->GetValue() == wxString("HID"))
            {
                bubble->setBootPortName(wxString("HID"));
            }
            else if (comboBootPortName->GetValue() == wxString("HID2"))
            {
                bubble->setBootPortName(wxString("HID2"));
            }
            else
            {
                wxString strCommRealName = wxString("//./") + comboBootPortName->GetValue();
                bubble->setBootPortName(strCommRealName);
            }
        }
    }
}


void BubbleHardwareManager::onComboBootPortNameChanged(wxCommandEvent &event)
{
    int idx = comboBootPortName->GetSelection();
    assert(wxNOT_FOUND == -1); // just in case that changes...
    fmt::println("combo changed to {}", idx);
    ctx.dispatch(hardware::select_port { idx });
    
    setPortType();
}


void BubbleHardwareManager::onComboBoardNameChanged(wxCommandEvent &event)
{
    if (comboBoardName)
    {
        if (bubble && parent)
        {
            bool currentComponentSaved = bubble->isSaved();
            wxString currentComponentName = bubble->getComponentFilesPath() + wxString(".mbqc"); //##Unhardcode

            //Find new seleted board's properties:
            for (const auto &board : boardsProperties)
            {
                if (board.name == event.GetString())
                {
                    currentBoardProperties = &board; // TODO: check use after free
                    break;
                }
            }
            bubble->setBoardName(event.GetString(), parent);
            changeImage();
            //##Debug:
            //wxMessageDialog dialog0(bubble->getParent(), getCurrentBoardProperties()->getPortType(),
            //                                             getCurrentBoardProperties()->getName()); //##Debug.
            //dialog0.ShowModal(); //##Debug.

            if ( ((getCurrentBoardProperties())->portType == wxString("HID")) || //##Unhardcode
                    ((getCurrentBoardProperties())->portType == wxString("HID2"))
                )
            {
                setPortSelectorEnabled(false);
                setPortNameString((getCurrentBoardProperties())->portType);
            }
            else
            {
                if (comboBootPortName)
                {
                    wxString selectedPort = comboBootPortName->GetValue();
                    setPortSelectorEnabled(true);
                    updatePorts();
                    if ( (comboBootPortName->GetValue() == wxString("HID")) || //##Unhardcode
                            (comboBootPortName->GetValue() == wxString("HID2"))
                        )
                    {
                        setPortNameString(wxString(""));
                    }
                }
            }

            //Updates the generated code (for example, with the include files):
            // bubble->loadBoardRelations(); TODO: check if it's ok to skip this

            if (getCurrentBoardProperties())
            {
                if (buttonGoToDriversDir)
                {
                    //Are there device drivers for this board?
                    buttonGoToDriversDir->Enable(getCurrentBoardProperties()->driverPath != wxString(""));

                    //Are there links for this board?
                    if (lblURL0)
                    {
                        lblURL0->SetURL(getCurrentBoardProperties()->url0);
                        lblURL0->SetLabel(getCurrentBoardProperties()->url0);
                        lblURL0->SetPosition(wxPoint(buttonGoToDriversDir->GetPosition().x, lblURL0->GetPosition().y));
                    }
                    if (lblURL1)
                    {
                        lblURL1->SetURL(getCurrentBoardProperties()->url1);
                        lblURL1->SetLabel(getCurrentBoardProperties()->url1);
                        lblURL1->SetPosition(wxPoint(buttonGoToDriversDir->GetPosition().x, lblURL1->GetPosition().y));
                    }
                }
            }

            ((MainFrame*)parent)->createComponent(true);
            if (currentComponentSaved)
                ((MainFrame*)parent)->loadFileComponent(currentComponentName);
            if ( ((MainFrame*)parent)->isShowingGeneratedCode() )
            {
                ((MainFrame*)parent)->toggleGeneratedCode();
                ((MainFrame*)parent)->toggleGeneratedCode();
            }
        }
    }
}
