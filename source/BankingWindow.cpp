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
// -- this will later work with the database and it's controller
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
    
    // TODO
    // - NAVIGATION BAR ON BOTTOM OF APP
    // - HEADER WITH PROFILE ICON AND SEARCH BUTTON
	// - ALERTS PANEL FOR NOTIFICATIONS (recent transactions, messages from bank, etc)
	// - COLLAPSABLE ACCOUNTS LIST WITH NAME OF ACCOUNT, ID, BALANCE (total of all accounts under)
    // - SEPARATE LIST FOR CREDIT CARDS

	int currentView = 0; // 0 = home, 1 = transfers, 2 = bills, 3 = advice, 4 = more, 5 = profile 6 = settings

	// -- Navigation Bar --
	// -- Lies at the bottom of the window with navigation buttons
	QHBoxLayout* navLayout = new QHBoxLayout();
	mainLayout->addStretch();
	mainLayout->addLayout(navLayout);

	QPushButton* homeBtn = new QPushButton("Home");
    homeBtn->setFixedHeight(70);
    styleNavigationButton(homeBtn);
    connect(homeBtn, &QPushButton::clicked, [this]() {
        std::cout << "Home button clicked" << std::endl;
    });
	navLayout->addWidget(homeBtn);

	QPushButton* transfersButton = new QPushButton("Transfers");
	transfersButton->setFixedHeight(70);
    styleNavigationButton(transfersButton);
	navLayout->addWidget(transfersButton);

	QPushButton* paymentsButton = new QPushButton("Bills");
	paymentsButton->setFixedHeight(70);
    styleNavigationButton(paymentsButton);
	navLayout->addWidget(paymentsButton);

	QPushButton* adviceButton = new QPushButton("Advice");
	adviceButton->setFixedHeight(70);
    styleNavigationButton(adviceButton);
	navLayout->addWidget(adviceButton);

	QPushButton* moreButton = new QPushButton("More");
	moreButton->setFixedHeight(70);
    styleNavigationButton(moreButton);
	navLayout->addWidget(moreButton);
}

void BankingWindow::initializeData() {
    currentCustomer.setUser(currentUser);
    currentCustomer.addAccount(currentAccount);
    
    //welcomeLabel->setText(QString("Welcome, %1!").arg(QString::fromStdString(currentUser.name())));
    
    //updateAccountSelector();
    //updateCurrentAccountDisplay();
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

// Add this helper function to your BankingWindow class (in the private section of BankingWindow.h)
void BankingWindow::styleNavigationButton(QPushButton* button) {
    // Detect theme automatically
    QPalette palette = this->palette();
    QColor windowColor = palette.color(QPalette::Window);
    bool isDarkMode = windowColor.lightness() < 128;
    
    if (isDarkMode) {
        button->setStyleSheet(
            "QPushButton {"
            "    border: none;"
            "    background-color: transparent;"
            "    color: #ffffff;"
            "    font-size: 14px;"
            "    font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "    background-color: rgba(255, 255, 255, 0.1);"
            "}"
            "QPushButton:pressed {"
            "    background-color: rgba(255, 255, 255, 0.2);"
            "}"
        );
    } else {
        button->setStyleSheet(
            "QPushButton {"
            "    border: none;"
            "    background-color: transparent;"
            "    color: #333333;"
            "    font-size: 14px;"
            "    font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "    background-color: rgba(0, 0, 0, 0.1);"
            "}"
            "QPushButton:pressed {"
            "    background-color: rgba(0, 0, 0, 0.2);"
            "}"
        );
    }
}
