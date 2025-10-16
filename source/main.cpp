// Brandon Coesta 10/10/2025

#include "Account.h"
#include "Customer.h"
#include "Card.h"
#include "User.h"
#include "Authenticator.h"
#include <iostream>
#include <string>
#include <chrono>
#include <algorithm>
// Qt includes
#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTextEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QGroupBox>
#include <QSpinBox>
#include <QDoubleSpinBox>

using namespace std;

// Forward declarations for console functions
void generateMiniStatement(const Account& account);
string switchAccount(Customer& customer);

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
    BankingWindow(QWidget* parent = nullptr) : QWidget(parent),
        currentUser(1, "John Doe", "jd@gmail.com", "reallygoodpassword123", "705-671-7171"),
        currentCustomer(1),
        currentAccount("123456789", AccountType::CHEQUING)
    {
        setupUI();
        initializeData();
    }

private slots:
    void onAccountChanged() {
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

    void onViewBalance() {
        balanceLabel->setText(QString("Current Balance: $%1").arg(currentAccount.getBalance(), 0, 'f', 2));
        outputArea->append(QString("Account balance: $%1").arg(currentAccount.getBalance(), 0, 'f', 2));
    }

    void onDeposit() {
        bool ok;
        double amount = QInputDialog::getDouble(this, "Deposit", "Enter deposit amount:", 0.0, 0.0, 999999.99, 2, &ok);
        if (ok && amount > 0) {
            currentAccount.deposit(amount);
            updateCurrentAccountDisplay();
            outputArea->append(QString("Deposited $%1 to account %2")
                .arg(amount, 0, 'f', 2)
                .arg(QString::fromStdString(currentAccount.accountNumber())));
            
            // Update the account in customer's list
            updateAccountInCustomer();
        }
    }

    void onWithdraw() {
        bool ok;
        double amount = QInputDialog::getDouble(this, "Withdraw", "Enter withdrawal amount:", 0.0, 0.0, currentAccount.getBalance(), 2, &ok);
        if (ok && amount > 0) {
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

    void onTransfer() {
        auto accountsList = currentCustomer.accounts();
        if (accountsList.size() < 2) {
            QMessageBox::information(this, "Transfer", "You need at least 2 accounts to transfer funds.");
            return;
        }

        QStringList accountNumbers;
        for (const auto& account : accountsList) {
            if (account.accountNumber() != currentAccount.accountNumber()) {
                accountNumbers << QString::fromStdString(account.accountNumber());
            }
        }

        bool ok;
        QString toAccount = QInputDialog::getItem(this, "Transfer", "Select destination account:", accountNumbers, 0, false, &ok);
        if (ok && !toAccount.isEmpty()) {
            double amount = QInputDialog::getDouble(this, "Transfer", "Enter transfer amount:", 0.0, 0.0, currentAccount.getBalance(), 2, &ok);
            if (ok && amount > 0) {
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

    void onViewTransactions() {
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

    void onMiniStatement() {
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

    void onNewAccount() {
        QStringList accountTypes;
        accountTypes << "Chequing" << "Savings" << "Credit";
        
        bool ok;
        QString selectedType = QInputDialog::getItem(this, "New Account", "Select account type:", accountTypes, 0, false, &ok);
        if (ok) {
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

private:
    void setupUI() {
        setWindowTitle("GSBS - Greater Sudbury Banking Service");
        setGeometry(100, 100, 800, 600);
        
        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        
        // Welcome section
        welcomeLabel = new QLabel();
        welcomeLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50; margin: 10px;");
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
    
    void initializeData() {
        currentCustomer.setUser(currentUser);
        currentCustomer.addAccount(currentAccount);
        
        welcomeLabel->setText(QString("Welcome, %1!").arg(QString::fromStdString(currentUser.name())));
        
        updateAccountSelector();
        updateCurrentAccountDisplay();
    }
    
    void updateAccountSelector() {
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
    
    void updateCurrentAccountDisplay() {
        currentAccountLabel->setText(QString("Account: %1").arg(QString::fromStdString(currentAccount.accountNumber())));
        balanceLabel->setText(QString("Balance: $%1").arg(currentAccount.getBalance(), 0, 'f', 2));
    }
    
    void updateAccountInCustomer() {
        // This is a workaround since we can't directly update the account in the customer's list
        // In a real application, this would be handled by the database layer
        currentCustomer.removeAccount(currentAccount.accountNumber());
        currentCustomer.addAccount(currentAccount);
    }
};

// Console function implementations
void generateMiniStatement(const Account& account) {
    cout << "Mini Statement for Account: " << account.accountNumber() << endl;
    cout << "----------------------------------------" << endl;
    cout << "Date\t\t\tType\tAmount\tDescription" << endl;
    cout << "----------------------------------------" << endl;
    
    // Get current time and calculate 4 weeks ago
    auto now = std::chrono::system_clock::now();
    auto fourWeeksAgo = now - std::chrono::hours(24 * 28); // 28 days = 4 weeks
    
    for (const auto& transaction : account.transactionHistory()) {
        // Check if transaction date is within the last 4 weeks
        if(transaction.rawDate() >= fourWeeksAgo && transaction.rawDate() <= now) {
            cout << transaction.date() << "\t"
                 << (transaction.type() == TransactionType::DEPOSIT ? "Deposit" : "Withdrawal") << "\t"
                 << "$" << transaction.amount() << "\t"
                 << transaction.description() << endl;
        }
    }
    cout << "----------------------------------------" << endl;
    cout << "Current Balance: $" << account.getBalance() << endl;
    cout << "----------------------------------------" << endl;
}

string switchAccount(Customer& customer) {
    auto accountsList = customer.accounts();
    if (accountsList.empty()) {
        cout << "No accounts available." << endl;
        return "";
    }
    cout << "Select an account:" << endl;
    int accountNum = 1;
    for (const auto& account : accountsList) {
        cout << accountNum << ". " << account.accountNumber() << endl;
        accountNum++;
    }
    int choice = 0;
    cout << "Enter the number of the account to select: ";
    cin >> choice;
    if (choice < 1 || choice >= accountNum) {
        cout << "Invalid selection." << endl;
        return "";
    }
    auto it = accountsList.begin();
    std::advance(it, choice - 1);
    cout << "Switched to account: " << it->accountNumber() << endl;
    return it->accountNumber();
}

// Keep the original console logic for reference
void consoleAppLogic() {
    string currentInput;

    // Dummy user and account for demonstration purposes
    // We will eventually pull this from a database
    User currentUser(1, "John Doe", "jd@gmail.com", "reallygoodpassword123", "705-671-7171");
    Customer currentCustomer(1);
    currentCustomer.setUser(currentUser);
    Account currentAccount("123456789", AccountType::CHEQUING);  // Changed to CHECKING
    currentCustomer.addAccount(currentAccount);

    cout << "-- GREATER SUDBURY BANKING SERVICE --" << endl;
    // display user information
    cout << "Welcome, " << currentUser.name() << "!" << endl;

    // show accounts - Fixed version
    auto accountsList = currentCustomer.accounts();
    int accountNum = 1;
    for (const auto& account : accountsList) {
        cout << "Account " << accountNum << ": " << account.accountNumber() << endl;
        accountNum++;
    }

    bool running = true;
    while (running)
    {
        cout << "Enter command (help for list of commands): ";
        getline(cin >> ws, currentInput);
        if (currentInput == "help")
        {
            cout << "Available commands:" << endl;
            cout << " help - Show this help message" << endl;
            cout << " view balance - Show selected account balance" << endl;
            cout << " view accounts - List all your accounts" << endl;
            cout << " view transactions - Show recent transactions for selected account" << endl;
            cout << " deposit - Deposit funds into selected account" << endl;
            cout << " withdraw - Withdraw funds from selected account" << endl;
            cout << " transfer - Transfer funds between accounts" << endl;
            cout << " new account - Open a new account" << endl;
            cout << " mini statement - Generate a mini statement for selected account" << endl;
            cout << " exit - Exit the application" << endl;
            // Add more commands as needed
        }
        else if (currentInput == "exit")
        {
            running = false;
            cout << "Exiting the application. Goodbye!" << endl;
        }
        else if (currentInput == "view balance")
        {
            // Placeholder for viewing balance logic
            cout << "Account balance: $" << currentAccount.getBalance() << endl;
        }
        else if (currentInput == "deposit") {
            cout << "How much would you like to deposit? ";
            double amnt = 0;  // Changed from int to double
            cin >> amnt;
            currentAccount.deposit(amnt);
        }
        else if (currentInput == "withdraw")
        {
            cout << "How much would you like to withdraw? ";
            double amnt = 0;  // Changed from int to double
            cin >> amnt;
            currentAccount.withdraw(amnt);
        }
        else if (currentInput == "transfer funds")
        {
            // Placeholder for transferring funds
        }
        else if (currentInput == "view accounts")
        {
            auto accountsList = currentCustomer.accounts();
            int accountNum = 1;
            for (const auto& account : accountsList) {
                cout << "Account " << accountNum << ": " << account.accountNumber() << endl;
                accountNum++;
            }
        }
        else if (currentInput == "view transactions")
        {
            cout << "Recent transactions for account " << currentAccount.accountNumber() << ":" << endl;
            for (const auto& transaction : currentAccount.transactionHistory()) {
                cout << transaction.date() << " - " << (transaction.type() == TransactionType::DEPOSIT ? "Deposit" : "Withdrawal")
                    << ": $" << transaction.amount() << endl;
            }
        }
        else if (currentInput == "new account")
        {
            // Placeholder for opening a new account
            cout << "Feature not implemented yet." << endl;
        }
        else if (currentInput == "switch account")
        {
            string selectedAccountNumber = switchAccount(currentCustomer);
            if (!selectedAccountNumber.empty()) {
                // Find the account in the customer's accounts
                auto accountsList = currentCustomer.accounts();
                auto it = std::find_if(accountsList.begin(), accountsList.end(),
                    [&selectedAccountNumber](const Account& account) {
                        return account.accountNumber() == selectedAccountNumber;
                    });
                if (it != accountsList.end()) {
                    currentAccount = *it;
                }
            }
        }
        else if (currentInput == "mini statement")
        {
            generateMiniStatement(currentAccount);
        }
        else
        {
            cout << "Unknown command: " << currentInput << endl;
            cout << "Type 'help' for a list of available commands." << endl;
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Authenticator test
    Authenticator auth;

    // Login Window
    QWidget loginWindow;
    loginWindow.resize(320, 240);
    loginWindow.setWindowTitle("GSBS | Login");
    loginWindow.show();

    QLabel* loginLabel = new QLabel("Sign in to GSBS", &loginWindow);
    loginLabel->setGeometry(60, 10, 200, 30);
    loginLabel->setAlignment(Qt::AlignCenter);
    loginLabel->show();

    // Username and Password fields
    QLineEdit* usernameField = new QLineEdit(&loginWindow);
    usernameField->setPlaceholderText("Username");
    usernameField->setGeometry(50, 50, 200, 30);
    usernameField->show();

    QLineEdit* passwordField = new QLineEdit(&loginWindow);
    passwordField->setPlaceholderText("Password");
    passwordField->setEchoMode(QLineEdit::Password);
    passwordField->setGeometry(50, 90, 200, 30);
    passwordField->show();

    // Show/Hide Password button
    QPushButton *showPasswordButton = new QPushButton("Show", &loginWindow);
    QObject::connect(showPasswordButton, &QPushButton::clicked, [passwordField, showPasswordButton]() {
        if (passwordField->echoMode() == QLineEdit::Password) {
            passwordField->setEchoMode(QLineEdit::Normal);
            showPasswordButton->setText("Hide");
        } else {
            passwordField->setEchoMode(QLineEdit::Password);
            showPasswordButton->setText("Show");
        }
    });
    showPasswordButton->setGeometry(260, 90, 60, 30);
    showPasswordButton->show();

    // Banking window (initially hidden)
    BankingWindow* bankingWindow = new BankingWindow();

    // Login button
    QPushButton *button = new QPushButton("Login", &loginWindow);
    QObject::connect(button, &QPushButton::clicked, [&auth, usernameField, passwordField, &loginWindow, bankingWindow]() {
        QString Qusername = usernameField->text();
        QString Qpassword = passwordField->text();
        string uname = Qusername.toStdString();
        string pword = Qpassword.toStdString();
        if(auth.verifyCredentials(uname, pword)){
            // Close login panel and show banking window
            loginWindow.close();
            bankingWindow->show();
        }
        else {
            QMessageBox::warning(&loginWindow, "Login Failed", "Invalid credentials. Please try again.");
        }
    });
    button->setGeometry(110, 140, 100, 30);
    button->show();

    return app.exec();
}

#include "main.moc"
