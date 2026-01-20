#include <cmath>
#include <QDebug>

#include "CalcLogic.h"


CalcLogic::CalcLogic(QObject *parent)
    : QObject(parent)
{}

QString CalcLogic::getCurrentNumberString() const {
    return currentNumberString_private;
}

QString CalcLogic::getCalculationString() const {
    return calculationString_private;
}

void CalcLogic::insertDigit(int digit)
{
    if (planDeleteCalculation){
        calculationString_private = "";
        emit calculationStringChanged();
        currentNumber = 0;
        planDeleteCalculation = false;
    }

    if (decimalCounter > 0){
        currentNumber += digit / std::pow(10.0, decimalCounter);
        decimalCounter++;
    }
    else {
        currentNumber = currentNumber * 10 + digit;
    }

    currentNumberChanged();
    lastWas.push_back(DIGIT);
    justDeleted = false;
    isCurrentNumberPushed = false;
}

void CalcLogic::insertDecimal(){
    if (planDeleteCalculation){
        calculationString_private = "";
        emit calculationStringChanged();
        planDeleteCalculation = false;
    }

    if (decimalCounter == 0){
        decimalCounter = 1;
        currentNumberString_private.append(".");
        emit currentNumberStringChanged();

        lastWas.push_back(DP);
        justDeleted = false;
    }
}

void CalcLogic::currentNumberChanged(){
    currentNumberString_private = QString::number(currentNumber);
    emit currentNumberStringChanged();
}

void CalcLogic::insertParth(const QString &par){

    if (planDeleteCalculation){
        calculationString_private = "";
        emit calculationStringChanged();
        planDeleteCalculation = false;
    }

    if (par == ")" && neededClosedPar == 0){
        return;
    }

    if(par == "("){
        neededClosedPar++;
    }

    if (par == ")"){
        neededClosedPar--;
    }

    storedOperands.push_back(par);

    if (par == ")" && !isCurrentNumberPushed){
        calculationString_private.append(QString::number(currentNumber));

        pushCurrentNumber();

        isCurrentNumberPushed = true;
    }
    calculationString_private.append(whiteChar + par + whiteChar);
    emit calculationStringChanged();

    lastWas.push_back(PARTH);
}

void CalcLogic::insertOperation(const QString &op)
{
    if (planDeleteCalculation){
        calculationString_private = "";
        emit calculationStringChanged();
        planDeleteCalculation = false;
    }

    if (!storedOperands.empty() && !storedValues.empty() && storedOperands.back() == DIV && storedValues.back() == 0) {
        currentNumberString_private = "Error: division by zero";
        storedValues.pop_back();
        decimalHistory.pop_back();

        emit currentNumberStringChanged();
        return;
    }

    if (!isCurrentNumberPushed){
        calculationString_private.append(QString::number(currentNumber) + whiteChar + op + whiteChar);

        pushCurrentNumber();
    }
    else {
        calculationString_private.append(whiteChar + op + whiteChar);
    }

    storedOperands.push_back(op);

    emit calculationStringChanged();

    lastWas.push_back(OPERAND);
    justDeleted = false;
}

void CalcLogic::pushCurrentNumber(){
    storedValues.push_back(currentNumber);
    currentNumber = 0;
    currentNumberChanged();

    decimalHistory.push_back(decimalCounter);
    decimalCounter = 0;
}


void CalcLogic::backspace(){
    if(lastWas.empty()){
        return;
    }

    if (lastWas.back() == Items::PARTH){
        storedOperands.pop_back();
        calculationString_private.chop(3);
        emit calculationStringChanged();

        if(lastWas[lastWas.size()-2] == DIGIT){
            restoreCurrentNumber();
            }
    }

    if(lastWas.back() == Items::DP){
        decimalCounter = 0;

        currentNumberString_private.removeLast();
        emit currentNumberStringChanged();
    }

    if(lastWas.back() == Items::DIGIT){
        if (decimalCounter > 0){
            decimalCounter--;
            currentNumber = std::trunc(currentNumber * std::pow(10.0, decimalCounter-1)) / std::pow(10.0, decimalCounter-1);
        }
        else{
            currentNumber = std::floor(currentNumber/10);
        }

        currentNumberString_private = QString::number(currentNumber);

        if (decimalCounter == 1){
            currentNumberString_private.append(".");
        }
        emit currentNumberStringChanged();
    }

    if(lastWas.back() == Items::OPERAND){
        storedOperands.pop_back();
        calculationString_private.chop(3);
        emit calculationStringChanged();

        if (lastWas[lastWas.size() - 2] == DIGIT){
            restoreCurrentNumber();
        }
        else{
            currentNumber = 0;
            decimalCounter = 0;
        }
    }

    lastWas.pop_back();
    justDeleted = true;
}

int CalcLogic::numDigits(int number)
{
    int digits = 0;
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}

void CalcLogic::restoreCurrentNumber(){
    currentNumber = storedValues.back();
    currentNumberChanged();
    storedValues.pop_back();

    decimalCounter = decimalHistory.back();
    decimalHistory.pop_back();

    calculationString_private.chop(numDigits(currentNumber * std::pow(10.0, decimalCounter)));

    emit calculationStringChanged();
}

void CalcLogic::calculate()
{
    if (planDeleteCalculation || neededClosedPar > 0 || lastWas.back() == Items::DP) {
        return;
    }

    // co kdyz ) 4444

    planDeleteCalculation = true;

    if (lastWas.back() == Items::PARTH){
        calculationString_private.append(" =");
    }
    else{
        storedValues.push_back(currentNumber);
        calculationString_private.append(QString::number(currentNumber) + " =");
    }


    calculateRec(0, storedValues.size() - 1, 0);

    currentNumber = storedValues[0];
    currentNumberChanged();

    storedValues.clear();
    storedOperands.clear();
    lastWas.clear();
    decimalHistory.clear();
    justDeleted = false;

    emit calculationStringChanged();
}

int CalcLogic::calculateRec(int startIndexVal, int endIndexVal, int startIndexOp) {
    int deletedValues = 0;
    for (int i = startIndexOp; i < storedOperands.size(); i++){
        if (storedOperands[i] == "("){
            deletedValues = calculateRec(startIndexVal+i, endIndexVal, i+1);
            storedOperands.erase(storedOperands.begin() + i);
        }
        if (storedOperands[i] == ")"){
            storedOperands.erase(storedOperands.begin() + i);
            endIndexVal = i - 1;
        }
    }

    deletedValues += calculateMultAndDiv(startIndexVal, endIndexVal - deletedValues, startIndexOp);

    if(storedOperands.size() == 0){
        return 0;
    }

    deletedValues += calculateAddAndSub(startIndexVal, endIndexVal - deletedValues, startIndexOp);

    return deletedValues;
}

int CalcLogic::calculateMultAndDiv(int startIndexVal, int endIndexVal, int startIndexOp){
    int deletedValues = 0;

    for(; startIndexVal < endIndexVal; startIndexVal++){
        if(storedOperands[startIndexOp] == MULT || storedOperands[startIndexOp] == DIV){
            if(storedOperands[startIndexOp] == MULT){
                storedValues[startIndexVal] *= storedValues[startIndexVal+1];
            }
            else{
                storedValues[startIndexVal] /= storedValues[startIndexVal+1];
            }

            storedOperands.erase(storedOperands.begin() + startIndexOp);
            storedValues.erase(storedValues.begin() + startIndexVal + 1);
            deletedValues++;

            startIndexVal--;
            endIndexVal--;
        }
        startIndexOp++;
    }

    return deletedValues;
}

int CalcLogic::calculateAddAndSub(int startIndexVal, int endIndexVal, int startIndexOp){
    int deletedValues = 0;
    for (; startIndexVal < endIndexVal; endIndexVal--) {
        if (storedOperands[startIndexOp] == ADD){
            storedValues[startIndexVal] += storedValues[startIndexVal + 1];
        }
        else{
            storedValues[startIndexVal] -= storedValues[startIndexVal + 1];
        }

        storedOperands.erase(storedOperands.begin() + startIndexOp);
        storedValues.erase(storedValues.begin() + startIndexVal + 1);
        deletedValues++;
    }

    return deletedValues;
}

void CalcLogic::clear()
{
    currentNumber = 0;
    currentNumberChanged();

    decimalCounter = 0;
    decimalHistory.clear();

    storedValues.clear();
    storedOperands.clear();

    lastWas.clear();
    neededClosedPar = 0;
    justDeleted = false;

    calculationString_private = "";
    emit calculationStringChanged();
}
