// Brandon Cotesta 10/16/2025

#include "Account.h"
#include "Customer.h"
#include "User.h"
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>

class BankingWindow : public QWidget {
    Q_OBJECT

private:
    User currentUser;
    Customer currentCustomer;
    Account currentAccount;
    
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
    void initializeData();
    void updateAccountSelector();
    void updateCurrentAccountDisplay();
    void updateAccountInCustomer();
};