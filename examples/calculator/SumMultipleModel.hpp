#pragma once

#include <QtCore/QObject>
#include <QtCore/QJsonObject>
#include <QtWidgets/QLabel>

#include <nodes/NodeDataModel>
#include <nodes/Connection>

#include <iostream>
#include <vector>

class DecimalData;

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class SumMultipleDataModel : public NodeDataModel
{
	Q_OBJECT
public:
	virtual ~SumMultipleDataModel() = default;

public:

	unsigned int
		nPorts(PortType portType) const override;

	NodeDataType
		dataType(PortType portType,
			PortIndex portIndex) const override;

	std::shared_ptr<NodeData>
		outData(PortIndex port) override;

	void
		setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

	QWidget*
		embeddedWidget() override { return nullptr; }

	NodeValidationState
		validationState() const override;

	QString
		validationMessage() const override;

	QString
		caption() const override
	{
		return QStringLiteral("Sum Multiple");
	}

	QString
		name() const override
	{
		return QStringLiteral("Sum Multiple");
	}


protected:

	virtual void
		compute();

protected:

	std::vector<std::weak_ptr<DecimalData>> _numberList;
	std::shared_ptr<DecimalData> _result;

	NodeValidationState modelValidationState = NodeValidationState::Warning;
	QString modelValidationError = QString("Missing or incorrect inputs");
};