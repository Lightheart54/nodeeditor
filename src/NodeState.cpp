#include "NodeState.hpp"

#include "NodeDataModel.hpp"

#include "Connection.hpp"

using QtNodes::NodeState;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::Connection;

NodeState::
NodeState(std::unique_ptr<NodeDataModel> const &model)
  : _inConnections(model->nPorts(PortType::In))
  , _outConnections(model->nPorts(PortType::Out))
  , _reaction(NOT_REACTING)
  , _reactingPortType(PortType::None)
  , _resizing(false)
{}


std::vector<NodeState::ConnectionPtrSet> const &
NodeState::
getEntries(PortType portType) const
{
  if (portType == PortType::In)
    return _inConnections;
  else
    return _outConnections;
}


std::vector<NodeState::ConnectionPtrSet> &
NodeState::
getEntries(PortType portType)
{
  if (portType == PortType::In)
    return _inConnections;
  else
    return _outConnections;
}


NodeState::ConnectionPtrSet
NodeState::
connections(PortType portType, PortIndex portIndex) const
{
  auto const &connections = getEntries(portType);

  return connections[portIndex];
}


void
NodeState::
addPort(PortType portType,
    PortIndex portIndex)
{
    if (portIndex < 0) return;

    auto& connections = getEntries(portType);

    if (portIndex > connections.size()) return;

    NodeState::ConnectionPtrSet newSet;
    if(portIndex < connections.size())
    {
        //we're inserting into the list
        connections.insert(connections.begin() + portIndex, newSet);
    }
    else
    {
        //we're appending to the end of the list
        connections.push_back(newSet);
    }
}

void
NodeState::
removePort(PortType portType,
    PortIndex portIndex)
{
    if (portIndex < 0) return;

    auto& connections = getEntries(portType);

    if (portIndex >= connections.size()) return;

    if (portIndex < connections.size() - 1)
    {
        //we're removing a connection internal to the list
        connections.erase(connections.begin() + portIndex);
    }
    else
    {
        //we're removing a connection at the end of the list
        connections.pop_back();
    }
}

void
NodeState::
setConnection(PortType portType,
              PortIndex portIndex,
              Connection& connection)
{
  auto &connections = getEntries(portType);

  connections.at(portIndex).insert(std::make_pair(connection.id(),
                                               &connection));
}


void
NodeState::
eraseConnection(PortType portType,
                PortIndex portIndex,
                QUuid id)
{
  getEntries(portType)[portIndex].erase(id);
}


NodeState::ReactToConnectionState
NodeState::
reaction() const
{
  return _reaction;
}


PortType
NodeState::
reactingPortType() const
{
  return _reactingPortType;
}


NodeDataType
NodeState::
reactingDataType() const
{
  return _reactingDataType;
}


void
NodeState::
setReaction(ReactToConnectionState reaction,
            PortType reactingPortType,
            NodeDataType reactingDataType)
{
  _reaction = reaction;

  _reactingPortType = reactingPortType;

  _reactingDataType = std::move(reactingDataType);
}


bool
NodeState::
isReacting() const
{
  return _reaction == REACTING;
}


void
NodeState::
setResizing(bool resizing)
{
  _resizing = resizing;
}


bool
NodeState::
resizing() const
{
  return _resizing;
}
