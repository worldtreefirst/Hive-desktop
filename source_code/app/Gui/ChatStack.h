#ifndef GUICHATSTACK_H
#define GUICHATSTACK_H

#include "GlobalData.h"
#include "UsrData.h"
#include "Data/AppDataManager.h"
#include "Gui/Frameworks/ScrollStack.h"
#include "Gui/Frameworks/Widgets/LabelButton.h"
#include "Gui/Frameworks/Widgets/AvatarWidgets.h"
#include "Gui/Frameworks/Widgets/TextBubble.h"

#include <QFileDialog>
#include <QScrollBar>
#include <QScrollArea>
#include <QTextEdit>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QJsonArray>
#include <QProgressBar>
#include <QVariantAnimation>

class GuiTextEdit;
class ChatStack_chat_widget;
class GuiChatStack_message_editor;
class GuiChatStack;


class GuiTextEdit : public QTextEdit
{
  Q_OBJECT

public:
  explicit GuiTextEdit(QWidget *parent = 0);
  ~GuiTextEdit();

protected:
///Do not delete!
//  void keyPressEvent(QKeyEvent *e);
//  void keyReleaseEvent(QKeyEvent *e);
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);

private:
  bool control_pressed = false;

signals:
  void keyEnterTriggered(bool pressed);
};



//////////////////////////mid//////////////////////////////////////
class ChatStack_chat_widget : public QWidget
{
  Q_OBJECT

public:
  explicit ChatStack_chat_widget(QString hello, QWidget *parent = 0);
  ~ChatStack_chat_widget();

  void clearChatBubbles();

private:
  QVBoxLayout *main_layout;
  QVBoxLayout *chat_bubble_layout;
  TextBubble *gui_chat_bubble;

  QList<TextBubble*> chat_bubble_list;

public slots:
  void addChatBubble(const QString &message, const bool &fromMe);

};


//////////////////////////bottom//////////////////////////////////////
class GuiChatStack_message_editor : public QWidget
{
  Q_OBJECT

public:
  explicit GuiChatStack_message_editor(QWidget *parent = 0);
  ~GuiChatStack_message_editor();

  QString currentFileName() {return current_file_name;}

  GuiTextEdit *text_editor;
  LabelButton *send_btn;

protected:
/// An old way to capture QEvent outside the class!
  bool eventFilter(QObject *obj, QEvent *e);

private:

  QVBoxLayout *edit_layout;
  QHBoxLayout *tool_layout;
  QHBoxLayout *main_layout;

  LabelButton *expression_label;
  LabelButton *image_label;
  LabelButton *file_label;
  QProgressBar *file_progress_bar;
  QLabel *file_progress_label;

  QVariantAnimation *file_tran_ani;

  QString current_file_name;

  bool control_pressed = false;
  bool file_label_hovered = false;

signals:
  void sendFile();
  void sendTriggered();

};



class GuiChatStack : public ScrollStack
{
  Q_OBJECT

public:
  explicit GuiChatStack(QWidget *parent = 0);
  ~GuiChatStack();

  void display(const QString &usrKey);
  bool refreshProfile(const QString &usrKey);
  bool refreshMessage(const QString &usrKey);

protected:
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);

  LayoutStyle layout_style = LayoutStyle::Profile;

private:
  //UI
  QHash<QString, ChatStack_chat_widget*> chat_widget_hash;
  QHash<QString, QString> editing_message_hash;

  ChatStack_chat_widget *chat_widget;
  GuiChatStack_message_editor *message_editor;

  //Data
  UsrData *usr_data;
  QPair<int, int> file_tran_progress_pair;

  int click_num = 0;
  QTime first_click_time;
  bool easter_animating = false;
  bool key_lock = false;

  //Function
  void flipUnreadMessage();
  void flipLatestMessage(const bool &clear);
  void flipUpMessage(const bool &clear);
  void flipDownMessage(const bool &clear);
  bool isDisplaying(const QString &usrKey);
  void setUsrData(UsrData *usrData);
  void updateFileTranProgress();

public slots:
  void onSendButtonClicked();
  void onKeyEnterTriggered(bool &pressed);

signals:
  void sendMessage(QString usrKey, QString message);

};






#endif // GUICHATSTACK_H
