// Types.h
#pragma once
#include <string>
#include <chrono> // For date and time handling

enum class AccountType { CHEQUING, SAVINGS, CREDIT, LOAN };
enum class TransactionType { DEPOSIT, WITHDRAWAL, TRANSFER, PAYMENT };
enum class CardType { DEBIT, CREDIT };
enum class InvestmentType { STOCK, ETF, CRYPTO, GIC, MUTUAL_FUND };
enum class RequestStatus { OPEN, PENDING, CLOSED };

using DateTime = std::chrono::system_clock::time_point;
