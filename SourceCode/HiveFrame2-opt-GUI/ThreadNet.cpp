#include "ThreadNet.h"

ThreadNet::ThreadNet(QObject *parent) : QThread(parent)
{
  udp_socket = new QUdpSocket(this);
  udp_socket->bind(udp_port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
  connect(udp_socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

  this->setParent(parent);
}

ThreadNet::~ThreadNet()
{
  running = false;
  emit usrLeft(&GlobalData::g_my_profile.key_str);
}

////////run
void ThreadNet::run()
{
  while(running)
    {
      sendOnlineStatus();
      refreshLocalHostIP();

      msleep(1000);
    }
}

void ThreadNet::setStatus(bool running)
{
  running = running;
}

void ThreadNet::refreshLocalHostIP()
{
  QList<QHostAddress> AddressList = QNetworkInterface::allAddresses();
  QHostAddress result;
  foreach(QHostAddress address, AddressList)
    {
      if(address.protocol() == QAbstractSocket::IPv4Protocol
         && address != QHostAddress::Null
         && address != QHostAddress::LocalHost)
        {
          if (!address.toString().contains("127.0."))
            {
              result = address;
              break;
            }
        }
    }

  if(!result.isNull())
    {
      GlobalData::g_localHostIP = result.toString();
    }
  else
    {
      GlobalData::g_localHostIP = "";
    }
  qDebug()<<"@refreshLocalHostIP(): finished!";
}

void ThreadNet::sendOnlineStatus()
{
  if(GlobalData::g_localHostIP != "")
    {
      sendUsrEnter();
    }
}

/////process packet
void ThreadNet::processMessage(MessageStruct *messageStruct)
{
  if(messageStruct->reciever_key != GlobalData::g_my_profile.key_str)
    {
      if(messageStruct->sender_key != GlobalData::g_my_profile.key_str)
        {
          qDebug()<<"人家的事情我不管";
        }
      else
        {
          qDebug()<<"我发了消息："<<messageStruct->message_str;
          emit messageRecieved(messageStruct, true);
        }
    }
  else
    {
      if(messageStruct->sender_key == GlobalData::g_my_profile.key_str)
        {
          qDebug()<<"我发给自己的消息";
          emit messageRecieved(messageStruct, true);
        }
      else
        {
          qDebug()<<"别人发给我的："<<messageStruct->message_str;
          emit messageRecieved(messageStruct, false);
        }
    }
}

void ThreadNet::processUsrEnter(UsrProfileStruct *usrProfileStruct)
{


  if(usrProfileStruct->key_str == GlobalData::g_my_profile.key_str)
    {
      emit usrEnter(usrProfileStruct);
      qDebug()<<"UDP receive# Myself entered.";
    }
  else
    {
      qDebug()<<"UDP receive# Someone entered.";
      emit usrEnter(usrProfileStruct);
    }

}

void ThreadNet::processUsrLeft(QString *usrKey)
{
  if(usrKey == GlobalData::g_my_profile.key_str)
    {
      emit usrLeft(usrKey);

      qDebug()<<"UDP receive# Myself left.";
    }

  qDebug()<<"UDP receive# Someone left.";
  emit usrLeft(usrKey);
}

void ThreadNet::processFileName()
{

}

void ThreadNet::processRefuse()
{

}


/////////////public slots////////////
void ThreadNet::sendUsrEnter()
{
  QByteArray data;
  QDataStream out(&data, QIODevice::WriteOnly);

  out << UsrEnter;
  out << GlobalData::g_localHostIP;
  out << GlobalData::g_my_profile.key_str;
  out << GlobalData::g_my_profile.name_str;
  out << GlobalData::g_my_profile.avatar_str;

  qint64 f = udp_socket->writeDatagram(data, data.length(), QHostAddress::Broadcast, udp_port);

  qDebug()<<"@sendUsrEnter(): finished!";
  return;
}

void ThreadNet::sendMessage(QString usrKeyStr, QString message)
{
  QByteArray data;
  QDataStream out(&data, QIODevice::WriteOnly);

  if(message == "")
    {
      qDebug()<<"@sendMessage(): Message content empty!";
      return;
    }

  out << Message << usrKeyStr << GlobalData::g_my_profile.key_str << message;
  udp_socket->writeDatagram(data,data.length(),QHostAddress::Broadcast, udp_port);
}

void ThreadNet::TEST_sendMessage(QString to, QString from, QString message)
{
  QByteArray data;
  QDataStream out(&data, QIODevice::WriteOnly);

  if (message == "")
    {
      qDebug()<<"@sendMessage(): Message content empty!";
      return;
    }

  out << Message << to << from << message;
  udp_socket->writeDatagram(data,data.length(),QHostAddress::Broadcast, udp_port);
}



/////////////private slots////////////

void ThreadNet::processPendingDatagrams()
{
  while(udp_socket->hasPendingDatagrams())
    {
      QByteArray datagram;
      datagram.resize(udp_socket->pendingDatagramSize());
      udp_socket->readDatagram(datagram.data(), datagram.size());
      QDataStream in(&datagram, QIODevice::ReadOnly);

      int msgType;
      in >> msgType;

      switch(msgType)
      {
        case Message:
          {
            MessageStruct message;
            in >> message.reciever_key;
            in >> message.sender_key;
            in >> message.message_str;

            processMessage(&message);
            break;
          }
        case UsrEnter:
          {
            UsrProfileStruct usr_profile;
            in >> usr_profile.ip_str;
            in >> usr_profile.key_str;
            in >> usr_profile.name_str;
            in >> usr_profile.avatar_str;

            processUsrEnter(&usr_profile);
            break;
          }
        case UsrLeft:
          {


          }
        case Refuse:
          {

          }
        case FileName:
          {

          }
      }
    }
}
