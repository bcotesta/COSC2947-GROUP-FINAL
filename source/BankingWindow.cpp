// Brandon Cotesta 10/16/2025

#include "BankingWindow.h"
#include <algorithm>
#include <chrono>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QInputDialog>
#include <QMessageBox>

#include <iostream>

BankingWindow::BankingWindow(QWidget* parent) : QWidget(parent),
    // eventually pull this data from database
    currentUser(1, "John Doe", "jd@gmail.com", "reallygoodpassword123", "705-671-7171"),
    currentCustomer(1),
    currentAccount("123456789", AccountType::CHEQUING)
{
    setupUI();
    initializeData();
}

// -- onAccountChanged --
// -- Updates the currentAccount and refreshes the display
void BankingWindow::onAccountChanged() {
    QString selectedAccountNumber = accountSelector->currentData().toString();
    if (!selectedAccountNumber.isEmpty()) {
        auto accountsList = currentCustomer.accounts();
        auto it = std::find_if(accountsList.begin(), accountsList.end(),
            [&selectedAccountNumber](const Account& account) {
                return QString::fromStdString(account.accountNumber()) == selectedAccountNumber;
            });
        if (it != accountsList.end()) {
            currentAccount = *it;
            updateCurrentAccountDisplay();
        }
    }
}

// -- onViewBalance --
// -- Displays the current balance of the selected account
void BankingWindow::onViewBalance() {
    balanceLabel->setText(QString("Balance: $%1").arg(currentAccount.getBalance(), 0, 'f', 2));
    outputArea->append(QString("Account balance: $%1").arg(currentAccount.getBalance(), 0, 'f', 2));
}

// -- onDeposit --
// -- Prompts for deposit amount and updates the account
void BankingWindow::onDeposit() {
    bool ok = false;
    // Dialog modal
    QInputDialog dlg;
	dlg.setWindowTitle("Deposit");
	dlg.setLabelText("Enter deposit amount:");
	dlg.setInputMode(QInputDialog::DoubleInput);
	dlg.setDoubleRange(0.0, 999999.99);
	dlg.setDoubleDecimals(2);

    dlg.setFixedSize(200, 98);
	dlg.setParent(this, Qt::Dialog);

    ok = (dlg.exec() == QDialog::Accepted);
    
    if (ok && dlg.doubleValue() > 0) {
	    double amount = dlg.doubleValue();
        currentAccount.deposit(amount);
        updateCurrentAccountDisplay();
        outputArea->append(QString("Deposited $%1 to account %2")
            .arg(amount, 0, 'f', 2)
            .arg(QString::fromStdString(currentAccount.accountNumber())));
        
        // Update the account in customer's list
        updateAccountInCustomer();
    }
}

// -- onWithdraw --
// -- Prompts for withdrawal amount and updates the account
void BankingWindow::onWithdraw() {
    bool ok = false;
    // Dialog modal
    QInputDialog dlg;
    dlg.setWindowTitle("Withdraw");
    dlg.setLabelText("Enter withdrawl amount:");
    dlg.setInputMode(QInputDialog::DoubleInput);
    dlg.setDoubleRange(0.0, currentAccount.getBalance());
    dlg.setDoubleDecimals(2);

    dlg.setFixedSize(200, 98);
    dlg.setParent(this, Qt::Dialog);

    ok = (dlg.exec() == QDialog::Accepted);

    if (ok && dlg.doubleValue() > 0) {
		double amount = dlg.doubleValue();
        if (amount <= currentAccount.getBalance()) {
            currentAccount.withdraw(amount);
            updateCurrentAccountDisplay();
            outputArea->append(QString("Withdrew $%1 from account %2")
                .arg(amount, 0, 'f', 2)
                .arg(QString::fromStdString(currentAccount.accountNumber())));
            
            // Update the account in customer's list
            updateAccountInCustomer();
        } else {
            QMessageBox::warning(this, "Insufficient Funds", "Withdrawal amount exceeds current balance.");
        }
    }
}

// -- onTransfer --
// -- Prompts for transfer details and updates both accounts
void BankingWindow::onTransfer() {
    auto accountsList = currentCustomer.accounts();
	// edge case if you dont have a second account
    if (accountsList.size() < 2) {
        QMessageBox::information(nullptr, "Transfer", "You need at least 2 accounts to transfer funds.");
        return;
    }

	// Prepare list of account numbers excluding the current account
    QStringList accountNumbers;
    for (const auto& account : accountsList) {
        if (account.accountNumber() != currentAccount.accountNumber()) {
            accountNumbers << QString::fromStdString(account.accountNumber());
        }
    }

	// Prompt user to select destination account, and then an amount
	bool ok = false;
    // Dialog modal
    QInputDialog dlg;
    dlg.setWindowTitle("Transfer");
    dlg.setLabelText("Select destination account:");
    dlg.setInputMode(QInputDialog::TextInput);      // this is unused but needs to be set to prevent errors
	dlg.setComboBoxItems(accountNumbers);
	dlg.setComboBoxEditable(false);

	dlg.setModal(true);

    dlg.setFixedSize(200, 98);
    dlg.setParent(this, Qt::Dialog);

    ok = (dlg.exec() == QDialog::Accepted);
    
    if (ok && !dlg.textValue().isEmpty()) {
		QString toAccount = dlg.textValue();

        // Dialog modal
        QInputDialog dlg_i;
        dlg_i.setWindowTitle("Transfer");
        dlg_i.setLabelText("Enter transfer amount:");
        dlg_i.setInputMode(QInputDialog::DoubleInput);
        dlg_i.setDoubleRange(0.0, currentAccount.getBalance());
        dlg_i.setDoubleDecimals(2);

        dlg_i.setFixedSize(200, 98);
        dlg_i.setParent(this, Qt::Dialog);

        ok = (dlg_i.exec() == QDialog::Accepted);

        if (ok && dlg_i.doubleValue() > 0) {
			double amount = dlg_i.doubleValue();
            try {
                currentCustomer.transferFunds(currentAccount.accountNumber(), toAccount.toStdString(), amount);
                outputArea->append(QString("Transferred $%1 from %2 to %3")
                    .arg(amount, 0, 'f', 2)
                    .arg(QString::fromStdString(currentAccount.accountNumber()))
                    .arg(toAccount));
                
                // Refresh current account
                onAccountChanged();
            } catch (const std::exception& e) {
                QMessageBox::warning(this, "Transfer Error", QString::fromStdString(e.what()));
            }
        }
    }
}

// -- onViewTransactions --
// -- Displays recent transactions for the current account
void BankingWindow::onViewTransactions() {
    outputArea->clear();
    outputArea->append(QString("Recent transactions for account %1:").arg(QString::fromStdString(currentAccount.accountNumber())));
    outputArea->append("----------------------------------------");
    
    for (const auto& transaction : currentAccount.transactionHistory()) {
        QString transactionText = QString("%1 - %2: $%3")
            .arg(QString::fromStdString(transaction.date()))
            .arg(transaction.type() == TransactionType::DEPOSIT ? "Deposit" : "Withdrawal")
            .arg(transaction.amount(), 0, 'f', 2);
        outputArea->append(transactionText);
    }
}

// -- onMiniStatement --
// -- Displays a mini statement for the last 4 weeks
void BankingWindow::onMiniStatement() {
    outputArea->clear();
    outputArea->append(QString("Mini Statement for Account: %1").arg(QString::fromStdString(currentAccount.accountNumber())));
    outputArea->append("----------------------------------------");
    outputArea->append("Date\t\t\tType\tAmount\tDescription");
    outputArea->append("----------------------------------------");
    
    // Get current time and calculate 4 weeks ago
    auto now = std::chrono::system_clock::now();
    auto fourWeeksAgo = now - std::chrono::hours(24 * 28);
    
    for (const auto& transaction : currentAccount.transactionHistory()) {
        if(transaction.rawDate() >= fourWeeksAgo && transaction.rawDate() <= now) {
            QString transactionText = QString("%1\t%2\t$%3\t%4")
                .arg(QString::fromStdString(transaction.date()))
                .arg(transaction.type() == TransactionType::DEPOSIT ? "Deposit" : "Withdrawal")
                .arg(transaction.amount(), 0, 'f', 2)
                .arg(QString::fromStdString(transaction.description()));
            outputArea->append(transactionText);
        }
    }
    outputArea->append("----------------------------------------");
    outputArea->append(QString("Current Balance: $%1").arg(currentAccount.getBalance(), 0, 'f', 2));
    outputArea->append("----------------------------------------");
}

// -- onNewAccount --
// -- Prompts for new account type and creates it, and adds to customer
// this will later work with the database and it's controller
void BankingWindow::onNewAccount() {
    QStringList accountTypes;
    accountTypes << "Chequing" << "Savings" << "Credit";
    
    bool ok = false;
    // Dialog modal
    QInputDialog dlg;
    dlg.setWindowTitle("New Account");
    dlg.setLabelText("Select account type:");
    dlg.setInputMode(QInputDialog::TextInput);      // this is unused but needs to be set to prevent errors
    dlg.setComboBoxItems(accountTypes);
    dlg.setComboBoxEditable(false);

    dlg.setModal(true);

    dlg.setFixedSize(200, 98);
    dlg.setParent(this, Qt::Dialog);
    
	ok = (dlg.exec() == QDialog::Accepted);

    if (ok) {
		QString selectedType = dlg.textValue();
        
        // Generate a simple account number (in real app, this would be from database)
        QString newAccountNumber = QString::number(123456000 + currentCustomer.accounts().size() + 1);
        
        AccountType accType = AccountType::CHEQUING;
        if (selectedType == "Savings") accType = AccountType::SAVINGS;
        else if (selectedType == "Credit") accType = AccountType::CREDIT;
        
        Account newAccount(newAccountNumber.toStdString(), accType);
        currentCustomer.addAccount(newAccount);
        
        updateAccountSelector();
        outputArea->append(QString("Created new %1 account: %2").arg(selectedType, newAccountNumber));
    }
}

// -- setupUI --
// -- Initializes and arranges all UI components
void BankingWindow::setupUI() {
    setWindowTitle("GSBS - Greater Sudbury Banking Service");
	setGeometry(0, 0, 500, 800); // window is 500x800 pixels, located in the top-left corner of the monitor (0,0)
    setMinimumSize(500,800);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Welcome section
    welcomeLabel = new QLabel();
    welcomeLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #ffffff; margin: 10px;");
    mainLayout->addWidget(welcomeLabel);
    
    // Account selection section
    QGroupBox* accountGroup = new QGroupBox("Account Selection");
    QHBoxLayout* accountLayout = new QHBoxLayout(accountGroup);
    
    accountLayout->addWidget(new QLabel("Current Account:"));
    accountSelector = new QComboBox();
    connect(accountSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BankingWindow::onAccountChanged);
    accountLayout->addWidget(accountSelector);
    
    currentAccountLabel = new QLabel();
    currentAccountLabel->setStyleSheet("font-weight: bold; color: #27ae60;");
    accountLayout->addWidget(currentAccountLabel);
    
    balanceLabel = new QLabel();
    balanceLabel->setStyleSheet("font-weight: bold; color: #e74c3c;");
    accountLayout->addWidget(balanceLabel);
    
    mainLayout->addWidget(accountGroup);
    
    // Buttons section
    QGroupBox* actionsGroup = new QGroupBox("Banking Actions");
    QGridLayout* buttonLayout = new QGridLayout(actionsGroup);
    
    viewBalanceBtn = new QPushButton("View Balance");
    connect(viewBalanceBtn, &QPushButton::clicked, this, &BankingWindow::onViewBalance);
    buttonLayout->addWidget(viewBalanceBtn, 0, 0);
    
    depositBtn = new QPushButton("Deposit");
    connect(depositBtn, &QPushButton::clicked, this, &BankingWindow::onDeposit);
    buttonLayout->addWidget(depositBtn, 0, 1);
    
    withdrawBtn = new QPushButton("Withdraw");
    connect(withdrawBtn, &QPushButton::clicked, this, &BankingWindow::onWithdraw);
    buttonLayout->addWidget(withdrawBtn, 0, 2);
    
    transferBtn = new QPushButton("Transfer");
    connect(transferBtn, &QPushButton::clicked, this, &BankingWindow::onTransfer);
    buttonLayout->addWidget(transferBtn, 1, 0);
    
    viewTransactionsBtn = new QPushButton("View Transactions");
    connect(viewTransactionsBtn, &QPushButton::clicked, this, &BankingWindow::onViewTransactions);
    buttonLayout->addWidget(viewTransactionsBtn, 1, 1);
    
    miniStatementBtn = new QPushButton("Mini Statement");
    connect(miniStatementBtn, &QPushButton::clicked, this, &BankingWindow::onMiniStatement);
    buttonLayout->addWidget(miniStatementBtn, 1, 2);
    
    newAccountBtn = new QPushButton("New Account");
    connect(newAccountBtn, &QPushButton::clicked, this, &BankingWindow::onNewAccount);
    buttonLayout->addWidget(newAccountBtn, 2, 0);
    
    mainLayout->addWidget(actionsGroup);
    
    // Output area
    QGroupBox* outputGroup = new QGroupBox("Transaction Output");
    QVBoxLayout* outputLayout = new QVBoxLayout(outputGroup);
    
    outputArea = new QTextEdit();
    outputArea->setReadOnly(true);
    outputArea->setMaximumHeight(200);
    outputLayout->addWidget(outputArea);
    
    mainLayout->addWidget(outputGroup);
}

void BankingWindow::initializeData() {
    currentCustomer.setUser(currentUser);
    currentCustomer.addAccount(currentAccount);
    
    welcomeLabel->setText(QString("Welcome, %1!").arg(QString::fromStdString(currentUser.name())));
    
    updateAccountSelector();
    updateCurrentAccountDisplay();
}

void BankingWindow::updateAccountSelector() {
    accountSelector->clear();
    auto accountsList = currentCustomer.accounts();
    for (const auto& account : accountsList) {
        QString accountDisplay = QString("%1 (%2)")
            .arg(QString::fromStdString(account.accountNumber()))
            .arg(account.accountType() == AccountType::CHEQUING ? "Chequing" : 
                 account.accountType() == AccountType::SAVINGS ? "Savings" : "Credit");
        accountSelector->addItem(accountDisplay, QString::fromStdString(account.accountNumber()));
    }
}

void BankingWindow::updateCurrentAccountDisplay() {
    currentAccountLabel->setText(QString("Account: %1").arg(QString::fromStdString(currentAccount.accountNumber())));
    balanceLabel->setText(QString("Balance: $%1").arg(currentAccount.getBalance(), 0, 'f', 2));
}

void BankingWindow::updateAccountInCustomer() {
    // This is a workaround since we can't directly update the account in the customer's list
    // In a real application, this would be handled by the database layer
    currentCustomer.removeAccount(currentAccount.accountNumber());
    currentCustomer.addAccount(currentAccount);
}
