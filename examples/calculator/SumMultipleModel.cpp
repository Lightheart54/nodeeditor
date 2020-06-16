#include "SumMultipleModel.hpp"

#include "DecimalData.hpp"

unsigned int
SumMultipleDataModel::
nPorts(PortType portType) const
{
    unsigned int result;

    if (portType == PortType::In)
        result = _numberList.size()+1;
    else
        result = 1;

    return result;
}


NodeDataType
SumMultipleDataModel::
dataType(PortType, PortIndex) const
{
    return DecimalData().type();
}


std::shared_ptr<NodeData>
SumMultipleDataModel::
outData(PortIndex)
{
    return std::static_pointer_cast<NodeData>(_result);
}


void
SumMultipleDataModel::
setInData(std::shared_ptr<NodeData> data, PortIndex portIndex)
{
    auto numberData =
        std::dynamic_pointer_cast<DecimalData>(data);
    if (numberData != nullptr)
    {
        //we're assigning new data
        if (portIndex == _numberList.size())
        {
            std::weak_ptr<DecimalData> newData = numberData;
            _numberList.push_back(numberData);
            emit portAdded(PortType::In, _numberList.size());
        }
        else
        {
            _numberList[portIndex] = numberData;
        }
    }
    else
    {
        //we're unassigning data
        _numberList[portIndex] = numberData;
        //if this was the last piece of data we need to reduce the number of ports
        if (portIndex == _numberList.size() - 1)
        {
            _numberList.pop_back();
            emit portDeleted(PortType::In, _numberList.size());
        }
    }

    compute();
}
NodeValidationState
SumMultipleDataModel::
validationState() const
{
    return modelValidationState;
}


QString
SumMultipleDataModel::
validationMessage() const
{
    return modelValidationError;
}

void
SumMultipleDataModel::
compute()
{
    PortIndex const outPortIndex = 0;
    double result = 0.0;
    for (auto& portData : _numberList)
    {
        auto num = portData.lock();
        if (num)
        {
            result += num->number();
        }
    }
    _result = std::make_shared<DecimalData>(result);

    Q_EMIT dataUpdated(outPortIndex);
}