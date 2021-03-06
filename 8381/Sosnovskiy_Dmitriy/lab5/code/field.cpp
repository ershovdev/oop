#include "field.h"

Field::~Field()
{
    for(unsigned int i = 0; i < rowsAmount; i++)
    {
        for(unsigned int j = 0; j < columnsAmount; j++)
        {
            delete fieldModel[i][j];
        }
        delete [] fieldModel[i];
    }
    delete [] fieldModel;
}

Field::Field(unsigned int rowsAmount, unsigned int columnsAmount, unsigned int maximumEntitiesAmount):
    rowsAmount(rowsAmount),
    columnsAmount(columnsAmount),
    maximumEntitiesAmount(maximumEntitiesAmount)
{
    fieldModel = new FieldComponent**[rowsAmount];
    for(unsigned int i = 0; i < rowsAmount; i++)
    {
        fieldModel[i] = new FieldComponent*[columnsAmount];
        for(unsigned int j = 0; j < columnsAmount; j++)
        {
            fieldModel[i][j] = new FieldComponent();
        }
    }

    generateLandscapes();

    currentEntitiesAmount = 0;
}

Field::Field(const Field& field)
    :rowsAmount(field.rowsAmount),
     columnsAmount(field.columnsAmount),
     maximumEntitiesAmount(field.maximumEntitiesAmount),
     currentEntitiesAmount(field.currentEntitiesAmount)
{
    fieldModel = new FieldComponent**[rowsAmount];
    for(unsigned int i = 0; i < rowsAmount; i++)
    {
        fieldModel[i] = new FieldComponent*[columnsAmount];
        for(unsigned int j = 0; j < columnsAmount; j++)
        {
            fieldModel[i][j] = nullptr;
            if(field.fieldModel[i][j] != nullptr)
            {
                fieldModel[i][j] = field.fieldModel[i][j]->copy();
            }
        }
    }
}

Field::Field(Field&& field)
    :fieldModel(field.fieldModel),
     rowsAmount(field.rowsAmount),
     columnsAmount(field.columnsAmount),
     maximumEntitiesAmount(field.maximumEntitiesAmount),
     currentEntitiesAmount(field.currentEntitiesAmount)
{
    field.fieldModel = nullptr;
    field.rowsAmount = 0;
    field.columnsAmount = 0;

}

unsigned int Field::getRowsAmount() const { return rowsAmount; }

unsigned int Field::getColumnsAmount() const { return columnsAmount; }

void Field::setMaximumEntitiesAmount(unsigned int newMaximumEntitiesAmount)
{
    maximumEntitiesAmount = newMaximumEntitiesAmount;
}

void Field::placeCreature(unsigned int rowNumber, unsigned int columnNumber, ICreature *creature)
{
    if(currentEntitiesAmount == maximumEntitiesAmount && creature->getCreatureType() != BASE)
        throw std::length_error("Maximum entities amount reached");

    if(rowNumber >= rowsAmount ||
       columnNumber >= columnsAmount)
        throw std::out_of_range("Out of a field size");

    FieldComponent* targetCell = fieldModel[rowNumber][columnNumber];

    if(creature->getCreatureType() == BASE) targetCell->setLandscape(new Ground());

    if(targetCell->hasCreature()) throw std::invalid_argument("Cell is not emtpy");

    if(targetCell->canHoldCreature() == false) throw std::invalid_argument("Landscape is not appropriate");

    targetCell->setCreature(creature);

    if(creature->getCreatureType() != BASE)
        currentEntitiesAmount++;

    creature->setCoordinates(rowNumber, columnNumber);
}

void Field::moveUnit(unsigned int rowNumberFrom, unsigned int columnNumberFrom,
                     unsigned int rowNumberTo, unsigned int columnNumberTo,
                     unsigned int actorId)
{
    if(rowNumberFrom >= rowsAmount ||
       columnNumberFrom >= columnsAmount ||
       rowNumberTo >= rowsAmount ||
       columnNumberTo >= columnsAmount) throw std::out_of_range("Out of a field size");


    FieldComponent* cell = fieldModel[rowNumberFrom][columnNumberFrom];

    FieldComponent* targetCell = fieldModel[rowNumberTo][columnNumberTo];

    if(cell->hasCreature() == false) throw std::invalid_argument("No creature");

    if(cell->getCreature()->getPossessorId() != actorId) throw std::invalid_argument("Not this player move");

    if(cell->getCreature()->isMoveable() == true)
    {
        if(targetCell->canHoldCreature() == false) throw std::invalid_argument("This landscape type can'h hold creature");

        FieldComponent* cellMoveInto = fieldModel[rowNumberTo][columnNumberTo];
        if(cellMoveInto->hasCreature()) throw std::invalid_argument("Cell to move is not empty");
        else {
            cellMoveInto->setCreature(cell->getCreature());
            cell->setCreature(nullptr);

            cellMoveInto->getCreature()->setCoordinates(rowNumberTo, columnNumberTo);
        }
    }
    else throw std::invalid_argument("Object is not moveable");
}

void Field::commitFight(unsigned int rowNumberFrom, unsigned int columnNumberFrom,
                        unsigned int rowNumberTo, unsigned int columnNumberTo,
                        unsigned int actorId)
{
    if(rowNumberFrom >= rowsAmount ||
       columnNumberFrom >= columnsAmount ||
       rowNumberTo >= rowsAmount ||
       columnNumberTo >= columnsAmount) throw std::out_of_range("Out of a field size");

    FieldComponent* cellFrom = fieldModel[rowNumberFrom][columnNumberFrom];
    FieldComponent* cellTo = fieldModel[rowNumberTo][columnNumberTo];

    if(cellFrom->hasCreature() == false || cellTo->hasCreature() == false)
        throw std::invalid_argument("No creature");

    if(cellFrom->getCreature()->getPossessorId() != actorId) throw std::invalid_argument("Not this player move");

    if(cellFrom->getCreature()->getCreatureType() == BASE)
        throw std::invalid_argument("Base can't attack");

    IUnit* attackUnit = dynamic_cast<IUnit*>(cellFrom->getCreature());
    ICreature* defendCreature = cellTo->getCreature();



    //Mediator implementation
    FightMediator* fightMediator = new FightMediator;

    fightMediator->commitFight(attackUnit, defendCreature);
}

FieldComponent* Field::getCell(unsigned int rowNumber, unsigned int columnNumber)
{
    if(rowNumber >= rowsAmount ||
       columnNumber >= columnsAmount)
        throw std::out_of_range("Out of a field size");

    else return fieldModel[rowNumber][columnNumber];
}

QString Field::getAsString()
{
    QString returnable = nullptr;
    for(unsigned int i = 0; i < rowsAmount; i++)
    {
        for(int objectCnt = 0; objectCnt < 3; objectCnt++)
        {
            QString temp_str = nullptr;
            for(unsigned int j = 0; j < columnsAmount; j++)
            {
                FieldComponent* cell = getCell(i,j);
                QString temporary = nullptr;
                switch (objectCnt) {
                    case 0:
                        temporary.append(cell->getCreatureInfo());
                        while(temporary.size() <= 15) temporary.append(" ");
                        break;
                    case 1:
                        temporary.append(cell->getObjectInfo());
                        while(temporary.size() <= 15) temporary.append(" ");
                        break;
                    case 2:
                        temporary.append(cell->getLandscapeInfo());
                        while(temporary.size() <= 15) temporary.append(" ");
                        break;
                }
                temp_str += temporary;
            }
            returnable.append(temp_str);
            returnable.append("\n");
        }
        returnable.append("\n");
    }

    return returnable;
}


