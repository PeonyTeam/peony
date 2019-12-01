// Copyright (c) 2019 The PIVX developers
// Copyright (c) 2019 The CryptoDev developers
// Copyright (c) 2019 The peony developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PNY_CORE_NEW_GUI_PNYGUI_H
#define PNY_CORE_NEW_GUI_PNYGUI_H

#if defined(HAVE_CONFIG_H)
#include "config/pny-config.h"
#endif

#include <QMainWindow>
#include <QStackedWidget>
#include <QSystemTrayIcon>
#include <QLabel>

#include "qt/pny/navmenuwidget.h"
#include "qt/pny/topbar.h"
#include "qt/pny/dashboardwidget.h"
#include "qt/pny/send.h"
#include "qt/pny/receivewidget.h"
#include "qt/pny/addresseswidget.h"
#include "qt/pny/privacywidget.h"
#include "qt/pny/coldstakingwidget.h"
#include "qt/pny/masternodeswidget.h"
#include "qt/pny/snackbar.h"
#include "qt/pny/settings/settingswidget.h"
#include "qt/rpcconsole.h"


class ClientModel;
class NetworkStyle;
class Notificator;
class WalletModel;


/**
  PNY GUI main class. This class represents the main window of the PNY UI. It communicates with both the client and
  wallet models to give the user an up-to-date view of the current core state.
*/
class PNYGUI : public QMainWindow
{
    Q_OBJECT

public:
    static const QString DEFAULT_WALLET;

    explicit PNYGUI(const NetworkStyle* networkStyle, QWidget* parent = 0);
    ~PNYGUI();

    /** Set the client model.
        The client model represents the part of the core that communicates with the P2P network, and is wallet-agnostic.
    */
    void setClientModel(ClientModel* clientModel);


    void resizeEvent(QResizeEvent *event) override;
    void showHide(bool show);
    int getNavWidth();
signals:
    void themeChanged(bool isLightTheme, QString& theme);
    void windowResizeEvent(QResizeEvent* event);
public slots:
    void changeTheme(bool isLightTheme);
    void goToDashboard();
    void goToSend();
    void goToReceive();
    void goToAddresses();
    //void goToPrivacy();
    void goToMasterNodes();
    void goToColdStaking();
    void goToSettings();

    void connectActions();

    /** Get restart command-line parameters and request restart */
    void handleRestart(QStringList args);

    /** Notify the user of an event from the core network or transaction handling code.
       @param[in] title     the message box / notification title
       @param[in] message   the displayed text
       @param[in] style     modality and style definitions (icon and used buttons - buttons only for message boxes)
                            @see CClientUIInterface::MessageBoxFlags
       @param[in] ret       pointer to a bool that will be modified to whether Ok was clicked (modal only)
    */
    void message(const QString& title, const QString& message, unsigned int style, bool* ret = nullptr);
    void messageInfo(const QString& message);
    bool execDialog(QDialog *dialog, int xDiv = 3, int yDiv = 5);
    /** Open FAQ dialog **/
    void openFAQ(int section = 0);

    /** Show incoming transaction notification for new transactions. */
    void incomingTransaction(const QString& date, int unit, const CAmount& amount, const QString& type, const QString& address);
#ifdef ENABLE_WALLET
    /** Set the wallet model.
        The wallet model represents a bitcoin wallet, and offers access to the list of transactions, address book and sending
        functionality.
    */
    bool addWallet(const QString& name, WalletModel* walletModel);
    bool setCurrentWallet(const QString& name);
    void removeAllWallets();
#endif // ENABLE_WALLET

protected:

    void changeEvent(QEvent* e) override;
    void closeEvent(QCloseEvent* event) override;

    /*
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);
    bool eventFilter(QObject* object, QEvent* event);
     */

private:
    bool enableWallet;
    ClientModel* clientModel = nullptr;

    // Actions
    QAction* quitAction = nullptr;
    QAction* toggleHideAction = nullptr;


    // Frame
    NavMenuWidget *navMenu = nullptr;
    TopBar *topBar = nullptr;
    QStackedWidget *stackedContainer = nullptr;

    DashboardWidget *dashboard = nullptr;
    SendWidget *sendWidget = nullptr;
    ReceiveWidget *receiveWidget = nullptr;
    AddressesWidget *addressesWidget = nullptr;
    //PrivacyWidget *privacyWidget = nullptr;
    MasterNodesWidget *masterNodesWidget = nullptr;
    ColdStakingWidget *coldStakingWidget = nullptr;
    SettingsWidget* settingsWidget = nullptr;

    SnackBar *snackBar = nullptr;

    RPCConsole* rpcConsole = nullptr;

    //
    QSystemTrayIcon* trayIcon = nullptr;
    QMenu* trayIconMenu = nullptr;
    Notificator* notificator = nullptr;

    QLabel *op = nullptr;
    bool opEnabled = false;

    /** Create the main UI actions. */
    void createActions(const NetworkStyle* networkStyle);
    /** Create system tray icon and notification */
    void createTrayIcon(const NetworkStyle* networkStyle);
    /** Create system tray menu (or setup the dock menu) */
    void createTrayIconMenu();

    void showTop(QWidget *view);
    bool openStandardDialog(QString title = "", QString body = "", QString okBtn = "OK", QString cancelBtn = "CANCEL");

    /** Connect core signals to GUI client */
    void subscribeToCoreSignals();
    /** Disconnect core signals from GUI client */
    void unsubscribeFromCoreSignals();

private slots:
    /** Show window if hidden, unminimize when minimized, rise when obscured or show if hidden and fToggleHidden is true */
    void showNormalIfMinimized(bool fToggleHidden = false);

    /** Simply calls showNormalIfMinimized(true) for use in SLOT() macro */
    void toggleHidden();

    /** called by a timer to check if fRequestShutdown has been set **/
    void detectShutdown();

#ifndef Q_OS_MAC
    /** Handle tray icon clicked */
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
#endif

signals:
    /** Signal raised when a URI was entered or dragged to the GUI */
    void receivedURI(const QString& uri);
    /** Restart handling */
    void requestedRestart(QStringList args);

};


#endif //PNY_CORE_NEW_GUI_PNYGUI_H
