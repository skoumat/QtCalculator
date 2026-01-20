#ifndef CALCLOGIC_H
#define CALCLOGIC_H

#pragma once

#include <QObject>
#include <vector>

enum Items{
    DIGIT = 1,
    OPERAND = 2,
    DP = 3,
    PARTH = 4
};

inline std::string ADD = "+";
inline std::string SUB = "-";
inline std::string MULT = "*";
inline std::string DIV = "/";

class CalcLogic : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString calculationString READ getCalculationString NOTIFY calculationStringChanged)
    Q_PROPERTY(QString currentNumberString READ getCurrentNumberString NOTIFY currentNumberStringChanged)

public:
    explicit CalcLogic(QObject *parent = nullptr);

    QString getCurrentNumberString() const;
    QString getCalculationString() const;

    Q_INVOKABLE void insertDigit(int digit);
    Q_INVOKABLE void insertOperation(const QString &op);
    Q_INVOKABLE void insertParth(const QString &par);
    Q_INVOKABLE void insertDecimal();
    Q_INVOKABLE void calculate();
    Q_INVOKABLE void backspace();
    Q_INVOKABLE void clear();

signals:
    void calculationStringChanged();
    void currentNumberStringChanged();

private:
    char whiteChar = '.';
    int decimalCounter = 0;
    double currentNumber = 0;
    bool isCurrentNumberPushed = false;
    bool planDeleteCalculation = false;
    std::vector<Items> lastWas;
    bool justDeleted = false;
    int neededClosedPar = 0;

    QString currentNumberString_private = "0";
    QString calculationString_private = " ";

    std::vector<double> storedValues;
    std::vector<QString> storedOperands;
    std::vector<int> decimalHistory;

    int calculateRec(int, int, int);
    void pushCurrentNumber();
    void currentNumberChanged();
    void restoreCurrentNumber();
    int calculateMultAndDiv(int, int, int);
    int calculateAddAndSub(int, int, int);

    int numDigits(int);
};

#endif // CALCLOGIC_H
