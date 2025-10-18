// Brandon Cotesta 10/16/2025

#include "Account.h"
#include "Customer.h"
#include "User.h"
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>

class BankingWindow : public QWidget {
    Q_OBJECT

private:
    User currentUser;
    Customer currentCustomer;
    Account currentAccount;
    
    // UI Layout Elements
    QVBoxLayout* mainLayout;
    QStackedWidget* contentStack;
    QHBoxLayout* navLayout;
    QHBoxLayout* tbLayout;
    
    // View Widgets
    QWidget* homeView;
    QWidget* transfersView;
    QWidget* billsView;
    QWidget* adviceView;
    QWidget* moreView;
    QWidget* profileView;
    
    // UI Elements
    QLabel* welcomeLabel;
    QLabel* currentAccountLabel;
    QLabel* balanceLabel;
    QComboBox* accountSelector;
    QPushButton* viewBalanceBtn;
    QPushButton* depositBtn;
    QPushButton* withdrawBtn;
    QPushButton* transferBtn;
    QPushButton* viewTransactionsBtn;
    QPushButton* miniStatementBtn;
    QPushButton* newAccountBtn;
    QTextEdit* outputArea;

public:
    BankingWindow(QWidget* parent = nullptr);

private slots:
    void onAccountChanged();
    void onViewBalance();
    void onDeposit();
    void onWithdraw();
    void onTransfer();
    void onViewTransactions();
    void onMiniStatement();
    void onNewAccount();

private:
    void setupUI();
    void setupViews();
    void initializeData();
    void updateAccountSelector();
    void updateCurrentAccountDisplay();
    void updateAccountInCustomer();
    void styleNavigationButton(QPushButton* button);

    // For the navbar / view switching
    void setCurrentView(int index);
    int currentViewIndex;
};