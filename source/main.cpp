// Brandon Coesta 10/10/2025

#include "Account.h"
#include "Customer.h"
#include "Card.h"
#include "User.h"
#include "Authenticator.h"
#include "BankingWindow.h"

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
string switchAccount(Customer& customer);

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

void registerNewUser(string user, string pass, string email, string phone) {
	int id = 2; // This would be generated or retrieved from a database. 
    User newUser(id, user, pass, email, phone); // creates a new user with given info
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Authenticator
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
    QPushButton *loginButton = new QPushButton("Login", &loginWindow);
    QObject::connect(loginButton, &QPushButton::clicked, [&auth, usernameField, passwordField, &loginWindow, bankingWindow]() {
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
    loginButton->setGeometry(110, 140, 100, 30);
    loginButton->show();

	// Register new user button
	QPushButton* registerButton = new QPushButton("Register", &loginWindow);
    QObject::connect(registerButton, &QPushButton::clicked, [&auth, &loginWindow]() {
        // Simple registration dialog (username, password, email, phone)
        QDialog* registerDialog = new QDialog(&loginWindow);
        registerDialog->setWindowTitle("Register New User");
        registerDialog->setModal(true);
        registerDialog->resize(350, 280);
    
        QVBoxLayout* layout = new QVBoxLayout(registerDialog);
    
        // Username field
        QLabel* usernameLabel = new QLabel("Username:", registerDialog);
        QLineEdit* usernameInput = new QLineEdit(registerDialog);
        layout->addWidget(usernameLabel);
        layout->addWidget(usernameInput);
    
        // Password field
        QLabel* passwordLabel = new QLabel("Password:", registerDialog);
        QLineEdit* passwordInput = new QLineEdit(registerDialog);
        passwordInput->setEchoMode(QLineEdit::Password);
        layout->addWidget(passwordLabel);
        layout->addWidget(passwordInput);

        // Confirm Password field
        QLabel* cpasswordLabel = new QLabel("Confirm password:", registerDialog);
        QLineEdit* cpasswordInput = new QLineEdit(registerDialog);
        cpasswordInput->setEchoMode(QLineEdit::Password);
        layout->addWidget(cpasswordLabel);
        layout->addWidget(cpasswordInput);
    
        // Email field
        QLabel* emailLabel = new QLabel("Email:", registerDialog);
        QLineEdit* emailInput = new QLineEdit(registerDialog);
        layout->addWidget(emailLabel);
        layout->addWidget(emailInput);
    
        // Phone field
        QLabel* phoneLabel = new QLabel("Phone:", registerDialog);
        QLineEdit* phoneInput = new QLineEdit(registerDialog);
        layout->addWidget(phoneLabel);
        layout->addWidget(phoneInput);
    
        // Buttons
        QHBoxLayout* buttonLayout = new QHBoxLayout();
        QPushButton* submitButton = new QPushButton("Register", registerDialog);
        QPushButton* cancelButton = new QPushButton("Cancel", registerDialog);
        buttonLayout->addWidget(submitButton);
        buttonLayout->addWidget(cancelButton);
        layout->addLayout(buttonLayout);
    
        // Submit button logic
        QObject::connect(submitButton, &QPushButton::clicked, [registerDialog, usernameInput, passwordInput, cpasswordInput, emailInput, phoneInput]() {
            QString username = usernameInput->text();
            QString password = passwordInput->text();
			QString cpassword = cpasswordInput->text();
            QString email = emailInput->text();
            QString phone = phoneInput->text();
            
            // edge cases
            if (username.isEmpty() || password.isEmpty() || email.isEmpty() || phone.isEmpty()) {
                QMessageBox::warning(registerDialog, "Registration Failed", "All fields are required.");
                return;
            }

            if (password != cpassword) {
				QMessageBox::warning(registerDialog, "Registration Failed", "Passwords do not match.");
                return;
            }

            if (password.length() < 6) {
				QMessageBox::warning(registerDialog, "Registration Failed", "Password must be at least 6 characters long.");
            }

            if (!email.contains("@")) {
                QMessageBox::warning(registerDialog, "Registration Failed", "Please enter a valid email address.");
                return;
            }

            if (phone.length() < 10) { // simplistic phone validation of the format 7056717171
                QMessageBox::warning(registerDialog, "Registration Failed", "Please enter a valid phone number.");
				return;
            }
            
			// user registration logic
            registerNewUser(username.toStdString(), password.toStdString(), email.toStdString(), phone.toStdString());
            QMessageBox::information(registerDialog, "Success", "User registered successfully!");
            registerDialog->accept();
        });
    
        // Cancel button logic
        QObject::connect(cancelButton, &QPushButton::clicked, [registerDialog]() {
            registerDialog->reject();
        });
    
        registerDialog->exec();
        delete registerDialog;
    });
    registerButton->setGeometry(110, 180, 100, 30);
    registerButton->show();

    // -- DEBUGGING --
    loginWindow.close();
    bankingWindow->show();

    return app.exec();
}
