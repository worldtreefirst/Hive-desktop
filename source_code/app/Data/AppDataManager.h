#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "GlobalData.h"
#include "UsrData.h"

#include <QThread>
#include <QMutex>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QFileInfo>
#include <QStandardPaths>

#include <QFontDatabase>
#include <QHostInfo>
#include <QTimer>
#include <QUuid>

#include <unordered_map>
#include <forward_list>
#include <mutex>

class AppDataManager;

class AppDataManager final : public QObject
{
  Q_OBJECT

public:

  struct NetBuffer {
    char *ipAddr;
    char *data;
    BaseProtocol protocol;
  };
  explicit AppDataManager(QObject *parent = 0);
  ~AppDataManager();

  static bool pushInboundBuffer(NetBuffer *buffer);
  static bool pushOutboundBuffer(NetBuffer *buffer);

public slots:
  void onUsrEntered(const UsrProfileStruct &usrProfileStruct);
  void onUsrLeft(QString *usrKey);
  void onMessageCome(const Message::TextMessage &messageStruct, bool fromMe);
  void onUpdatesAvailable();

private:
  static std::forward_list<NetBuffer*> inbound_net_buffer;
  static std::mutex inbound_net_buffer_mutex;
  static std::forward_list<NetBuffer*> outbound_net_buffer;
  static std::mutex outbound_net_buffer_mutex;

  void checkSettings();
  inline bool checkDir(const QString &directory);

  /*!
   * Functions called by constructor AppDataManager::AppDataManager(), only called once when during initialization.
   */
  inline void initVariable();
  inline void checkFiles();
  inline void readSettings();
  inline void loadUsrList();
  inline void loadFonts();
  inline void loadUpdates();
  inline void loadTimerTasks();

  inline QString makeUuid();
  inline QJsonDocument makeDefaultSettings();
  inline QJsonDocument makeUpdateJson(const int version[]);

  inline void updateUsr(const UsrProfileStruct &usrProfileStruct);
  inline void deleteUsr(const QStringList usrInfoStrList);

  /*!
   * Maps for storing settings
   */
  std::map<QString, int*> settings_int_hash;
  std::map<QString, QColor*> settings_qcolor_hash;
  std::map<QString, QString*> settings_qstring_hash;
  std::map<QString, bool*> settings_bool_hash;

private slots:
  void writeSettings();

signals:
  void updatesAvailable();
  void usrProfileLoaded(UsrData *userData);
  void usrProfileChanged(UsrData *userData);
  void messageLoaded(Message::TextMessage messageStrList, bool fromMe);


};


#endif // DATAMANAGER_H
